/*
 * Copyright 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <vector>

#include "buffet/avahi_mdns_client.h"
#include "buffet/dbus_constants.h"

#include <avahi-common/defs.h>
#include <avahi-common/address.h>
#include <base/guid.h>
#include <chromeos/dbus/async_event_sequencer.h>
#include <chromeos/dbus/dbus_signal_handler.h>
#include <chromeos/dbus/dbus_method_invoker.h>
#include <chromeos/errors/error.h>
#include <dbus/object_path.h>
#include <dbus/object_proxy.h>

using chromeos::ErrorPtr;
using chromeos::dbus_utils::AsyncEventSequencer;
using chromeos::dbus_utils::CallMethodAndBlock;
using chromeos::dbus_utils::ExtractMethodCallResults;
using CompletionAction =
    chromeos::dbus_utils::AsyncEventSequencer::CompletionAction;

namespace buffet {

AvahiMdnsClient::AvahiMdnsClient(const scoped_refptr<dbus::Bus> &bus)
    : bus_(bus) {
}

AvahiMdnsClient::~AvahiMdnsClient() {
}

// NB: This should be the one-and-only definition of this MdnsClient static
// method.
std::unique_ptr<MdnsClient> MdnsClient::CreateInstance(
    const scoped_refptr<dbus::Bus> &bus) {
  return std::unique_ptr<MdnsClient>{new AvahiMdnsClient(bus)};
}

// TODO(rginda): Report errors back to the caller.
// TODO(rginda): Support publishing more than one service.
void AvahiMdnsClient::PublishService(
    const std::string& service_type, uint16_t port,
    const std::vector<std::string>& txt) {

  CHECK_EQ("_privet._tcp", service_type);

  if (service_state_ == READY) {
    if (service_type_ != service_type || port_ != port) {
      // If the type or port of a service changes we have to re-publish
      // rather than just update the txt record.
      StopPublishing(service_type_);
      if (service_state_ != UNDEF) {
        LOG(ERROR) << "Failed to disable existing service.";
        return;
      }
    }
  }

  service_type_ = service_type;
  port_ = port;
  txt_ = GetTxtRecord(txt);

  if (avahi_state_ == UNDEF || avahi_state_ == ERROR) {
    ConnectToAvahi();
  } else if (service_state_ == READY) {
    UpdateServiceTxt();
  } else if (avahi_state_ == READY) {
    CreateEntryGroup();
  } else {
    CHECK(avahi_state_ == PENDING);
  }
}

// TODO(rginda): If we support publishing more than one service then we
// may need a less ambiguous way of unpublishing them.
void AvahiMdnsClient::StopPublishing(const std::string& service_type) {
  if (service_type_ != service_type) {
    LOG(ERROR) << "Unknown service type: " << service_type;
    return;
  }

  if (service_state_ != READY) {
    LOG(ERROR) << "Service is not published.";
  }

  service_type_.clear();
  port_ = 0;

  FreeEntryGroup();
}

// Transform a service_info to a mDNS compatible TXT record value.
// Concretely, a TXT record consists of a list of strings in the format
// "key=value".  Each string must be less than 256 bytes long, since they are
// length/value encoded.  Keys may not contain '=' characters, but are
// otherwise unconstrained.
//
// We need a DBus type of "aay", which is a vector<vector<uint8_t>> in our
// bindings.
AvahiMdnsClient::TxtRecord AvahiMdnsClient::GetTxtRecord(
    const std::vector<std::string>& txt) {
  TxtRecord result;
  result.reserve(txt.size());
  for (const std::string& s : txt) {
    result.emplace_back();
    std::vector<uint8_t>& record = result.back();
    record.insert(record.end(), s.begin(), s.end());
  }
  return result;
}

void AvahiMdnsClient::ConnectToAvahi() {
  avahi_state_ = PENDING;

  avahi_ = bus_->GetObjectProxy(
      dbus_constants::avahi::kServiceName,
      dbus::ObjectPath(dbus_constants::avahi::kServerPath));

  // This callback lives for the lifetime of the ObjectProxy.
  avahi_->SetNameOwnerChangedCallback(
      base::Bind(&AvahiMdnsClient::OnAvahiOwnerChanged,
                 weak_ptr_factory_.GetWeakPtr()));

  // Reconnect to our signals on a new Avahi instance.
  scoped_refptr<AsyncEventSequencer> sequencer(new AsyncEventSequencer());
  chromeos::dbus_utils::ConnectToSignal(
      avahi_,
      dbus_constants::avahi::kServerInterface,
      dbus_constants::avahi::kServerSignalStateChanged,
      base::Bind(&AvahiMdnsClient::OnAvahiStateChanged,
                 weak_ptr_factory_.GetWeakPtr()),
      sequencer->GetExportHandler(
          dbus_constants::avahi::kServerInterface,
          dbus_constants::avahi::kServerSignalStateChanged,
          "Failed to subscribe to Avahi state change.",
          true));
  sequencer->OnAllTasksCompletedCall(
      {// Get a onetime callback with the initial state of Avahi.
       AsyncEventSequencer::WrapCompletionTask(
            base::Bind(&dbus::ObjectProxy::WaitForServiceToBeAvailable,
                       avahi_,
                       base::Bind(&AvahiMdnsClient::OnAvahiAvailable,
                                  weak_ptr_factory_.GetWeakPtr()))),
      });
}

void AvahiMdnsClient::CreateEntryGroup() {
  ErrorPtr error;

  service_state_ = PENDING;

  auto resp = CallMethodAndBlock(
      avahi_, dbus_constants::avahi::kServerInterface,
      dbus_constants::avahi::kServerMethodEntryGroupNew,
      &error);

  dbus::ObjectPath group_path;
  if (!resp || !ExtractMethodCallResults(resp.get(), &error, &group_path)) {
    service_state_ = ERROR;
    LOG(ERROR) << "Error creating group.";
    return;
  }
  entry_group_ = bus_->GetObjectProxy(dbus_constants::avahi::kServiceName,
                                      group_path);

  // If we fail to connect to the StateChange signal for this group, just
  // report that the whole thing has failed.
  auto on_connect_cb = [](const std::string& interface_name,
                          const std::string& signal_name,
                          bool success) {
    if (!success) {
      LOG(ERROR) << "Failed to connect to StateChange signal "
        "from EntryGroup.";
      return;
    }
  };

  chromeos::dbus_utils::ConnectToSignal(
      entry_group_,
      dbus_constants::avahi::kGroupInterface,
      dbus_constants::avahi::kGroupSignalStateChanged,
      base::Bind(&AvahiMdnsClient::HandleGroupStateChanged,
                 weak_ptr_factory_.GetWeakPtr()),
      base::Bind(on_connect_cb));

  CreateService();
}

void AvahiMdnsClient::FreeEntryGroup() {
  if (!entry_group_) {
    LOG(ERROR) << "No group to free.";
    return;
  }

  ErrorPtr error;
  auto resp = CallMethodAndBlock(entry_group_,
                                 dbus_constants::avahi::kGroupInterface,
                                 dbus_constants::avahi::kGroupMethodFree,
                                 &error);
  // Extract and log relevant errors.
  bool success = resp && ExtractMethodCallResults(resp.get(), &error);
  if (!success) {
    LOG(ERROR) << "Error freeing service group";
  }

  // Ignore any signals we may have registered for from this proxy.
  entry_group_->Detach();
  entry_group_ = nullptr;

  service_state_ = UNDEF;
}

void AvahiMdnsClient::CreateService() {
  ErrorPtr error;

  VLOG(1) << "CreateService: name: " << device_id_ << ", type: " <<
      service_type_ << ", port: " << port_;
  auto resp = CallMethodAndBlock(
      entry_group_,
      dbus_constants::avahi::kGroupInterface,
      dbus_constants::avahi::kGroupMethodAddService,
      &error,
      int32_t{AVAHI_IF_UNSPEC},
      int32_t{AVAHI_PROTO_UNSPEC},
      uint32_t{0},  // No flags.
      device_id_,
      std::string{"_privet._tcp"},
      std::string{},  // domain.
      std::string{},  // hostname
      port_,
      txt_);
  if (!resp || !ExtractMethodCallResults(resp.get(), &error)) {
    LOG(ERROR) << "Error creating service";
    service_state_ = ERROR;
    return;
  }

  resp = CallMethodAndBlock(entry_group_,
                            dbus_constants::avahi::kGroupInterface,
                            dbus_constants::avahi::kGroupMethodCommit,
                            &error);
  if (!resp || !ExtractMethodCallResults(resp.get(), &error)) {
    LOG(ERROR) << "Error committing service.";
    service_state_ = ERROR;
    return;
  }

  service_state_ = READY;
}

void AvahiMdnsClient::UpdateServiceTxt() {
  ErrorPtr error;

  CHECK_EQ(READY, service_state_);

  VLOG(1) << "UpdateServiceTxt: name " << device_id_ << ", type: " <<
      service_type_ << ", port: " << port_;
  auto resp = CallMethodAndBlock(
      entry_group_,
      dbus_constants::avahi::kGroupInterface,
      dbus_constants::avahi::kGroupMethodUpdateServiceTxt,
      &error,
      int32_t{AVAHI_IF_UNSPEC},
      int32_t{AVAHI_PROTO_UNSPEC},
      uint32_t{0},  // No flags.
      device_id_,
      std::string{"_privet._tcp"},
      std::string{},  // domain.
      txt_);
  if (!resp || !ExtractMethodCallResults(resp.get(), &error)) {
    LOG(ERROR) << "Error creating service";
    service_state_ = ERROR;
    return;
  }
};

void AvahiMdnsClient::OnAvahiOwnerChanged(const std::string& old_owner,
                                          const std::string& new_owner) {
  if (new_owner.empty()) {
    OnAvahiAvailable(false);
    return;
  }
  OnAvahiAvailable(true);
}

void AvahiMdnsClient::OnAvahiStateChanged(int32_t state,
                                          const std::string& error) {
  HandleAvahiStateChange(state);
}

void AvahiMdnsClient::OnAvahiAvailable(bool avahi_is_on_dbus) {
  VLOG(1) << "Avahi is "  << (avahi_is_on_dbus ? "up." : "down.");
  int32_t state = AVAHI_SERVER_FAILURE;
  if (avahi_is_on_dbus) {
    auto resp = CallMethodAndBlock(
        avahi_, dbus_constants::avahi::kServerInterface,
        dbus_constants::avahi::kServerMethodGetState,
        nullptr);
    if (!resp || !ExtractMethodCallResults(resp.get(), nullptr, &state)) {
      LOG(WARNING) << "Failed to get avahi initial state.  Relying on signal.";
    }
  }
  VLOG(1) << "Initial Avahi state=" << state << ".";
  HandleAvahiStateChange(state);
}

void AvahiMdnsClient::HandleAvahiStateChange(int32_t state) {
  switch (state) {
    case AVAHI_SERVER_RUNNING: {
      // All host RRs have been established.
      VLOG(1) << "Avahi ready for action.";
      if (avahi_state_ == READY) {
        LOG(INFO) << "Ignoring redundant Avahi up event.";
        return;
      }
      avahi_state_ = READY;
      CreateEntryGroup();
    } break;
    case AVAHI_SERVER_INVALID:
      // Invalid state (initial).
    case AVAHI_SERVER_REGISTERING:
      // Host RRs are being registered.
    case AVAHI_SERVER_COLLISION:
      // There is a collision with a host RR. All host RRs have been withdrawn,
      // the user should set a new host name via avahi_server_set_host_name().
    case AVAHI_SERVER_FAILURE:
      // Some fatal failure happened, the server is unable to proceed.
      avahi_state_ = ERROR;
      if (service_state_ != UNDEF)
        service_state_ = ERROR;

      LOG(ERROR) << "Avahi changed to error state: " << state;
      break;
    default:
      LOG(ERROR) << "Unknown Avahi server state change to " << state;
      break;
  }
}

void AvahiMdnsClient::HandleGroupStateChanged(
    int32_t state,
    const std::string& error_message) {
  if (state == AVAHI_ENTRY_GROUP_COLLISION ||
      state == AVAHI_ENTRY_GROUP_FAILURE) {
    LOG(ERROR) << "Avahi service group error: " << state;
  }
}

}  // namespace buffet
