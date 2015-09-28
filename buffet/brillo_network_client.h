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

#ifndef BUFFET_BRILLO_NETWORK_CLIENT_H_
#define BUFFET_BRILLO_NETWORK_CLIENT_H_

#include <memory>
#include <vector>

#include <base/cancelable_callback.h>

#include "connectivity_client.h"

#include "buffet/network_client.h"

namespace buffet {

class BrilloNetworkClient : public NetworkClient {
 public:
  explicit BrilloNetworkClient(const std::set<std::string>& device_whitelist);
  ~BrilloNetworkClient() override;

  // Implements the Network interface.
  void AddConnectionChangedCallback(
      const ConnectionChangedCallback& listener) override;
  void Connect(
      const std::string& ssid,
      const std::string& passphrase,
      const weave::SuccessCallback& on_success,
      const base::Callback<void(const weave::Error*)>& on_error) override;
  State GetConnectionState() const override;
  void StartAccessPoint(const std::string& ssid) override;
  void StopAccessPoint() override;

 private:
  enum class ConnectionState {
    kIdle,
    kInProgress,
    kConnected,
    kTimedOut
  };
  void OnConnectionTimeout();
  void ScheduleNextStatePoll();
  void UpdateConnectionState();

  ConnectivityClient connectivity_client_;
  std::vector<ConnectionChangedCallback> connection_listeners_;
  base::CancelableClosure connection_timeout_closure_;
  base::CancelableClosure periodic_connection_state_closure_;
  base::Closure connection_success_closure_;
  State connectivity_state_{State::kOffline};
  bool is_connected_;

  DISALLOW_COPY_AND_ASSIGN(BrilloNetworkClient);
};

}  // namespace buffet

#endif  // BUFFET_BRILLO_NETWORK_CLIENT_H_

