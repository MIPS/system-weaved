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

#ifndef BUFFET_PEERD_CLIENT_H_
#define BUFFET_PEERD_CLIENT_H_

#include <memory>
#include <string>
#include <vector>

#include <base/callback.h>
#include <base/memory/ref_counted.h>
#include <weave/provider/dns_service_discovery.h>

#include "peerd/dbus-proxies.h"

namespace dbus {
class Bus;
}  // namespace dbus

namespace buffet {

// Publishes privet service on mDns using peerd.
class PeerdClient : public weave::provider::DnsServiceDiscovery {
 public:
  explicit PeerdClient(const scoped_refptr<dbus::Bus>& bus);
  ~PeerdClient() override;

  // Mdns implementation.
  void PublishService(const std::string& service_type,
                      uint16_t port,
                      const std::vector<std::string>& txt) override;
  void StopPublishing(const std::string& service_type) override;

 private:
  void OnPeerdOnline(org::chromium::peerd::ManagerProxy* manager_proxy);
  void OnPeerdOffline(const dbus::ObjectPath& object_path);

  // Updates published information.  Removes service if HTTP is not alive.
  void Update();

  void ExposeService();
  void RemoveService();

  void UpdateImpl();

  org::chromium::peerd::ObjectManagerProxy peerd_object_manager_proxy_;
  // |peerd_manager_proxy_| is owned by |peerd_object_manager_proxy_|.
  org::chromium::peerd::ManagerProxy* peerd_manager_proxy_{nullptr};

  bool published_{false};
  uint16_t port_{0};
  std::vector<std::string> txt_;

  base::WeakPtrFactory<PeerdClient> restart_weak_ptr_factory_{this};
  base::WeakPtrFactory<PeerdClient> weak_ptr_factory_{this};

  DISALLOW_COPY_AND_ASSIGN(PeerdClient);
};

}  // namespace buffet

#endif  // BUFFET_PEERD_CLIENT_H_
