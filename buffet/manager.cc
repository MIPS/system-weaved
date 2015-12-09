// Copyright 2015 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "buffet/manager.h"

#include <map>
#include <set>
#include <string>

#include <base/bind.h>
#include <base/bind_helpers.h>
#include <base/files/file_enumerator.h>
#include <base/files/file_util.h>
#include <base/json/json_reader.h>
#include <base/json/json_writer.h>
#include <base/message_loop/message_loop.h>
#include <base/time/time.h>
#include <cutils/properties.h>
#include <brillo/bind_lambda.h>
#include <brillo/dbus/async_event_sequencer.h>
#include <brillo/dbus/exported_object_manager.h>
#include <brillo/errors/error.h>
#include <brillo/http/http_transport.h>
#include <brillo/http/http_utils.h>
#include <brillo/key_value_store.h>
#include <brillo/message_loops/message_loop.h>
#include <brillo/mime_utils.h>
#include <dbus/bus.h>
#include <dbus/object_path.h>
#include <dbus/values_util.h>
#include <weave/enum_to_string.h>

#include "brillo/weaved_system_properties.h"
#include "buffet/bluetooth_client.h"
#include "buffet/buffet_config.h"
#include "buffet/dbus_command_dispatcher.h"
#include "buffet/dbus_conversion.h"
#include "buffet/http_transport_client.h"
#include "buffet/mdns_client.h"
#include "buffet/shill_client.h"
#include "buffet/weave_error_conversion.h"
#include "buffet/webserv_client.h"

using brillo::dbus_utils::AsyncEventSequencer;
using brillo::dbus_utils::DBusMethodResponse;
using brillo::dbus_utils::ExportedObjectManager;

namespace buffet {

namespace {

const char kPairingSessionIdKey[] = "sessionId";
const char kPairingModeKey[] = "mode";
const char kPairingCodeKey[] = "code";

const char kErrorDomain[] = "buffet";
const char kFileReadError[] = "file_read_error";

bool LoadFile(const base::FilePath& file_path,
              std::string* data,
              brillo::ErrorPtr* error) {
  if (!base::ReadFileToString(file_path, data)) {
    brillo::errors::system::AddSystemError(error, FROM_HERE, errno);
    brillo::Error::AddToPrintf(error, FROM_HERE, kErrorDomain, kFileReadError,
                               "Failed to read file '%s'",
                               file_path.value().c_str());
    return false;
  }
  return true;
}

void LoadTraitDefinitions(const BuffetConfig::Options& options,
                          weave::Device* device) {
  // Load component-specific device trait definitions.
  base::FilePath dir{options.definitions.Append("traits")};
  LOG(INFO) << "Looking for trait definitions in " << dir.value();
  base::FileEnumerator enumerator(dir, false, base::FileEnumerator::FILES,
                                  FILE_PATH_LITERAL("*.json"));
  std::vector<std::string> result;
  for (base::FilePath path = enumerator.Next(); !path.empty();
       path = enumerator.Next()) {
    LOG(INFO) << "Loading trait definition from " << path.value();
    std::string json;
    CHECK(LoadFile(path, &json, nullptr));
    device->AddTraitDefinitionsFromJson(json);
  }
}

void LoadCommandDefinitions(const BuffetConfig::Options& options,
                            weave::Device* device) {
  auto load_packages = [device](const base::FilePath& root,
                                const base::FilePath::StringType& pattern) {
    base::FilePath dir{root.Append("commands")};
    LOG(INFO) << "Looking for command schemas in " << dir.value();
    base::FileEnumerator enumerator(dir, false, base::FileEnumerator::FILES,
                                    pattern);
    for (base::FilePath path = enumerator.Next(); !path.empty();
         path = enumerator.Next()) {
      LOG(INFO) << "Loading command schema from " << path.value();
      std::string json;
      CHECK(LoadFile(path, &json, nullptr));
      device->AddCommandDefinitionsFromJson(json);
    }
  };
  load_packages(options.definitions, FILE_PATH_LITERAL("*.json"));
  if (!options.test_definitions.empty())
    load_packages(options.test_definitions, FILE_PATH_LITERAL("*test.json"));
}

void LoadStateDefinitions(const BuffetConfig::Options& options,
                          weave::Device* device) {
  // Load component-specific device state definitions.
  base::FilePath dir{options.definitions.Append("states")};
  LOG(INFO) << "Looking for state definitions in " << dir.value();
  base::FileEnumerator enumerator(dir, false, base::FileEnumerator::FILES,
                                  FILE_PATH_LITERAL("*.schema.json"));
  std::vector<std::string> result;
  for (base::FilePath path = enumerator.Next(); !path.empty();
       path = enumerator.Next()) {
    LOG(INFO) << "Loading state definition from " << path.value();
    std::string json;
    CHECK(LoadFile(path, &json, nullptr));
    device->AddStateDefinitionsFromJson(json);
  }
}

void LoadStateDefaults(const BuffetConfig::Options& options,
                       weave::Device* device) {
  // Load component-specific device state defaults.
  base::FilePath dir{options.definitions.Append("states")};
  LOG(INFO) << "Looking for state defaults in " << dir.value();
  base::FileEnumerator enumerator(dir, false, base::FileEnumerator::FILES,
                                  FILE_PATH_LITERAL("*.defaults.json"));
  std::vector<std::string> result;
  for (base::FilePath path = enumerator.Next(); !path.empty();
       path = enumerator.Next()) {
    LOG(INFO) << "Loading state defaults from " << path.value();
    std::string json;
    CHECK(LoadFile(path, &json, nullptr));
    CHECK(device->SetStatePropertiesFromJson(json, nullptr));
  }
}

}  // anonymous namespace

class Manager::TaskRunner : public weave::provider::TaskRunner {
 public:
  void PostDelayedTask(const tracked_objects::Location& from_here,
                       const base::Closure& task,
                       base::TimeDelta delay) override {
    brillo::MessageLoop::current()->PostDelayedTask(from_here, task, delay);
  }
};

Manager::Manager(const Options& options,
                 const base::WeakPtr<ExportedObjectManager>& object_manager)
    : options_{options},
      dbus_object_(object_manager.get(),
                   object_manager->GetBus(),
                   com::android::Weave::ManagerAdaptor::GetObjectPath()) {}

Manager::~Manager() {
}

void Manager::Start(AsyncEventSequencer* sequencer) {
  RestartWeave(sequencer);

  dbus_adaptor_.RegisterWithDBusObject(&dbus_object_);
  dbus_registration_handler_ =
      sequencer->GetHandler("Manager.RegisterAsync() failed.", true);
}

void Manager::RestartWeave(AsyncEventSequencer* sequencer) {
  Stop();

  task_runner_.reset(new TaskRunner{});
  config_.reset(new BuffetConfig{options_.config_options});
  http_client_.reset(new HttpTransportClient);
  shill_client_.reset(new ShillClient{dbus_object_.GetBus(),
                                      options_.device_whitelist,
                                      !options_.xmpp_enabled});
  weave::provider::HttpServer* http_server{nullptr};
#ifdef BUFFET_USE_WIFI_BOOTSTRAPPING
  if (!options_.disable_privet) {
    mdns_client_ = MdnsClient::CreateInstance(dbus_object_.GetBus());
    web_serv_client_.reset(new WebServClient{
        dbus_object_.GetBus(), sequencer,
        base::Bind(&Manager::CreateDevice, weak_ptr_factory_.GetWeakPtr())});
    bluetooth_client_ = BluetoothClient::CreateInstance();
    http_server = web_serv_client_.get();

    if (options_.enable_ping) {
      auto ping_handler = base::Bind(
          [](std::unique_ptr<weave::provider::HttpServer::Request> request) {
            request->SendReply(brillo::http::status_code::Ok, "Hello, world!",
                               brillo::mime::text::kPlain);
          });
      http_server->AddHttpRequestHandler("/privet/ping", ping_handler);
      http_server->AddHttpsRequestHandler("/privet/ping", ping_handler);
    }
  }
#endif  // BUFFET_USE_WIFI_BOOTSTRAPPING

  if (!http_server)
    CreateDevice();
}

void Manager::CreateDevice() {
  if (device_)
    return;

  device_ = weave::Device::Create(config_.get(), task_runner_.get(),
                                  http_client_.get(), shill_client_.get(),
                                  mdns_client_.get(), web_serv_client_.get(),
                                  shill_client_.get(), bluetooth_client_.get());

  LoadTraitDefinitions(options_.config_options, device_.get());
  LoadCommandDefinitions(options_.config_options, device_.get());
  LoadStateDefinitions(options_.config_options, device_.get());
  LoadStateDefaults(options_.config_options, device_.get());

  device_->AddSettingsChangedCallback(
      base::Bind(&Manager::OnConfigChanged, weak_ptr_factory_.GetWeakPtr()));

  command_dispatcher_.reset(
      new DBusCommandDispacher{dbus_object_.GetObjectManager(), device_.get()});

  device_->AddStateChangedCallback(
      base::Bind(&Manager::OnStateChanged, weak_ptr_factory_.GetWeakPtr()));

  device_->AddGcdStateChangedCallback(
      base::Bind(&Manager::OnGcdStateChanged, weak_ptr_factory_.GetWeakPtr()));

  device_->AddPairingChangedCallbacks(
      base::Bind(&Manager::OnPairingStart, weak_ptr_factory_.GetWeakPtr()),
      base::Bind(&Manager::OnPairingEnd, weak_ptr_factory_.GetWeakPtr()));

  auto handler = dbus_registration_handler_;
  if (handler.is_null())
    handler = AsyncEventSequencer::GetDefaultCompletionAction();
  dbus_object_.RegisterAsync(handler);
  dbus_registration_handler_.Reset();
}

void Manager::Stop() {
  command_dispatcher_.reset();
  device_.reset();
#ifdef BUFFET_USE_WIFI_BOOTSTRAPPING
  web_serv_client_.reset();
  mdns_client_.reset();
#endif  // BUFFET_USE_WIFI_BOOTSTRAPPING
  shill_client_.reset();
  http_client_.reset();
  config_.reset();
  task_runner_.reset();
}

void Manager::RegisterDevice(DBusMethodResponsePtr<std::string> response,
                             const std::string& ticket_id) {
  LOG(INFO) << "Received call to Manager.RegisterDevice()";

  device_->Register(ticket_id, base::Bind(&Manager::RegisterDeviceDone,
                                          weak_ptr_factory_.GetWeakPtr(),
                                          base::Passed(&response)));
}

void Manager::RegisterDeviceDone(DBusMethodResponsePtr<std::string> response,
                                 weave::ErrorPtr error) {
  if (error) {
    brillo::ErrorPtr brillo_error;
    ConvertError(*error, &brillo_error);
    return response->ReplyWithError(brillo_error.get());
  }
  LOG(INFO) << "Device registered: " << device_->GetSettings().cloud_id;
  response->Return(device_->GetSettings().cloud_id);
}

void Manager::AddComponent(DBusMethodResponsePtr<> response,
                           const std::string& name,
                           const std::vector<std::string>& traits) {
  brillo::ErrorPtr brillo_error;
  weave::ErrorPtr error;
  if (!device_->AddComponent(name, traits, &error)) {
    ConvertError(*error, &brillo_error);
    return response->ReplyWithError(brillo_error.get());
  }
  response->Return();
}

void Manager::UpdateState(DBusMethodResponsePtr<> response,
                          const std::string& component,
                          const brillo::VariantDictionary& property_set) {
  brillo::ErrorPtr brillo_error;
  auto properties =
      DictionaryFromDBusVariantDictionary(property_set, &brillo_error);
  if (!properties)
    return response->ReplyWithError(brillo_error.get());

  weave::ErrorPtr error;
  if (!device_->SetStateProperties(component, *properties, &error)) {
    ConvertError(*error, &brillo_error);
    return response->ReplyWithError(brillo_error.get());
  }
  response->Return();
}

bool Manager::GetState(brillo::ErrorPtr* error, std::string* state) {
  const base::DictionaryValue& json = device_->GetState();
  base::JSONWriter::WriteWithOptions(
      json, base::JSONWriter::OPTIONS_PRETTY_PRINT, state);
  return true;
}

void Manager::AddCommand(DBusMethodResponsePtr<std::string> response,
                         const std::string& json_command) {
  std::string error_message;
  std::unique_ptr<base::Value> value(
      base::JSONReader::ReadAndReturnError(json_command, base::JSON_PARSE_RFC,
                                           nullptr, &error_message)
          .release());
  const base::DictionaryValue* command{nullptr};
  if (!value || !value->GetAsDictionary(&command)) {
    return response->ReplyWithError(FROM_HERE, brillo::errors::json::kDomain,
                                    brillo::errors::json::kParseError,
                                    error_message);
  }

  std::string id;
  weave::ErrorPtr error;
  if (!device_->AddCommand(*command, &id, &error)) {
    brillo::ErrorPtr brillo_error;
    ConvertError(*error, &brillo_error);
    return response->ReplyWithError(brillo_error.get());
  }

  response->Return(id);
}

std::string Manager::TestMethod(const std::string& message) {
  LOG(INFO) << "Received call to test method: " << message;
  return message;
}

void Manager::OnStateChanged() {
  const base::DictionaryValue& state = device_->GetState();
  std::string json;
  base::JSONWriter::WriteWithOptions(
      state, base::JSONWriter::OPTIONS_PRETTY_PRINT, &json);
  dbus_adaptor_.SetState(json);
}

void Manager::OnGcdStateChanged(weave::GcdState state) {
  std::string state_string = weave::EnumToString(state);
  dbus_adaptor_.SetStatus(state_string);
  property_set(weaved::system_properties::kState, state_string.c_str());
}

void Manager::OnConfigChanged(const weave::Settings& settings) {
  dbus_adaptor_.SetDeviceId(settings.cloud_id);
  dbus_adaptor_.SetOemName(settings.oem_name);
  dbus_adaptor_.SetModelName(settings.model_name);
  dbus_adaptor_.SetModelId(settings.model_id);
  dbus_adaptor_.SetName(settings.name);
  dbus_adaptor_.SetDescription(settings.description);
  dbus_adaptor_.SetLocation(settings.location);
}

void Manager::OnPairingStart(const std::string& session_id,
                             weave::PairingType pairing_type,
                             const std::vector<uint8_t>& code) {
  // For now, just overwrite the exposed PairInfo with
  // the most recent pairing attempt.
  dbus_adaptor_.SetPairingInfo(brillo::VariantDictionary{
      {kPairingSessionIdKey, session_id},
      {kPairingModeKey, weave::EnumToString(pairing_type)},
      {kPairingCodeKey, code},
  });
}

void Manager::OnPairingEnd(const std::string& session_id) {
  auto exposed_pairing_attempt = dbus_adaptor_.GetPairingInfo();
  auto it = exposed_pairing_attempt.find(kPairingSessionIdKey);
  if (it == exposed_pairing_attempt.end()) {
    return;
  }
  std::string exposed_session{it->second.TryGet<std::string>()};
  if (exposed_session == session_id) {
    dbus_adaptor_.SetPairingInfo(brillo::VariantDictionary{});
  }
}

}  // namespace buffet
