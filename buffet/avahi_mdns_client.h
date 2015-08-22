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

#ifndef BUFFET_AVAHI_MDNS_CLIENT_H_
#define BUFFET_AVAHI_MDNS_CLIENT_H_

#include <map>
#include <string>

#include <base/memory/weak_ptr.h>
#include <dbus/bus.h>
#include <weave/mdns.h>

#include "buffet/mdns_client.h"

namespace buffet {

// Publishes privet service on mDns using Avahi.
class AvahiMdnsClient : public MdnsClient {
 public:
  explicit AvahiMdnsClient(const scoped_refptr<dbus::Bus>& bus);
  ~AvahiMdnsClient() override;

  // weave::Mdns implementation.
  void PublishService(const std::string& service_type,
                      uint16_t port,
                      const std::map<std::string, std::string>& txt) override;
  void StopPublishing(const std::string& service_type) override;

 private:
  using TxtRecord = std::vector<std::vector<uint8_t>>;

  // States used to track progress of our asynchronous dbus operations.
  enum AsyncState {
    UNDEF,
    PENDING,
    READY,
    ERROR
  };

  scoped_refptr<dbus::Bus> bus_;
  dbus::ObjectProxy* avahi_{nullptr};
  // The avahi interface we use to add/remove mdns services.
  dbus::ObjectProxy* entry_group_{nullptr};

  // State of our dbus connection to avahi.
  AsyncState avahi_state_{UNDEF};
  // State of the group/service publish operation.
  AsyncState service_state_{UNDEF};

  std::string service_type_;
  uint16_t port_{0};
  TxtRecord txt_;

  // Must be last member to invalidate pointers before actual destruction.
  base::WeakPtrFactory<AvahiMdnsClient> weak_ptr_factory_{this};

  // Convert a {string:string} text record into something we can send over
  // dbus.
  TxtRecord GetTxtRecord(const std::map<std::string, std::string>& txt);

  void ConnectToAvahi();
  void CreateEntryGroup();
  void FreeEntryGroup();
  void CreateService();

  void OnAvahiOwnerChanged(const std::string& old_owner,
                           const std::string& new_owner);
  void OnAvahiStateChanged(int32_t state,
                           const std::string& error);
  void OnAvahiAvailable(bool avahi_is_on_dbus);
  void HandleAvahiStateChange(int32_t state);
  void HandleGroupStateChanged(int32_t state,
                               const std::string& error_message);
  DISALLOW_COPY_AND_ASSIGN(AvahiMdnsClient);
};

}  // namespace buffet

#endif  // BUFFET_AVAHI_MDNS_CLIENT_H_
