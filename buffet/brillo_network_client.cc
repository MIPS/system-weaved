/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include "buffet/brillo_network_client.h"

#include <base/message_loop/message_loop.h>

namespace buffet {

namespace {
const char kErrorDomain[] = "brillo_network";
const int kConnectionTimeoutSeconds = 30;
const int kConnectionActivePollSeconds = 3;
const int kConnectionInactivePollSeconds = 10;
}  // namespace

BrilloNetworkClient::BrilloNetworkClient(
    const std::set<std::string>& device_whitelist)
    : NetworkClient{device_whitelist}, state_(weave::NetworkState::kOffline) {
  UpdateConnectionState();
}

BrilloNetworkClient::~BrilloNetworkClient() {
}

void BrilloNetworkClient::AddConnectionChangedCallback(
    const ConnectionChangedCallback& listener) {
  connection_listeners_.push_back(listener);
}

void BrilloNetworkClient::Connect(const std::string& ssid,
                                  const std::string& passphrase,
                                  const weave::SuccessCallback& on_success,
                                  const weave::ErrorCallback& on_error) {
  if (!connectivity_client_.ConnectToAccessPoint(ssid, passphrase)) {
    weave::ErrorPtr error;
    weave::Error::AddTo(&error, FROM_HERE, kErrorDomain, "network_failure",
                        "Failed to connect to service");
    base::MessageLoop::current()->PostDelayedTask(
        FROM_HERE, base::Bind(on_error, base::Owned(error.release())), {});
    return;
  }

  connection_success_closure_ = on_success;
  state_ = weave::NetworkState::kConnecting;

  connection_timeout_closure_.Reset(
      base::Bind(&BrilloNetworkClient::OnConnectionTimeout,
                 base::Unretained(this)));
  base::MessageLoop::current()->PostDelayedTask(
      FROM_HERE,
      connection_timeout_closure_.callback(),
      base::TimeDelta::FromSeconds(kConnectionTimeoutSeconds));

  ScheduleNextStatePoll();
}

weave::NetworkState BrilloNetworkClient::GetConnectionState() const {
  return state_;
}

void BrilloNetworkClient::StartAccessPoint(const std::string& ssid) {
  connectivity_client_.EnableAccessPoint(ssid);
  state_ = weave::NetworkState::kOffline;
}

void BrilloNetworkClient::StopAccessPoint() {
  connectivity_client_.DisableAccessPoint();
}

void BrilloNetworkClient::OnConnectionTimeout() {
  state_ = weave::NetworkState::kFailure;
}

void BrilloNetworkClient::ScheduleNextStatePoll() {
  periodic_connection_state_closure_.Reset(
      base::Bind(&BrilloNetworkClient::UpdateConnectionState,
                 base::Unretained(this)));
  int poll_period_seconds;
  if (state_ == weave::NetworkState::kConnecting) {
    poll_period_seconds = kConnectionActivePollSeconds;
  } else {
    poll_period_seconds = kConnectionInactivePollSeconds;
  }
  base::MessageLoop::current()->PostDelayedTask(
      FROM_HERE,
      periodic_connection_state_closure_.callback(),
      base::TimeDelta::FromSeconds(poll_period_seconds));
}

void BrilloNetworkClient::UpdateConnectionState() {
  bool was_connected = state_ == weave::NetworkState::kConnected;
  bool is_connected = connectivity_client_.IsConnected();

  if (is_connected) {
    if (state_ == weave::NetworkState::kConnecting)
      connection_success_closure_.Run();
    state_ = weave::NetworkState::kConnected;
  } else if (state_ == weave::NetworkState::kConnected) {
    state_ = weave::NetworkState::kOffline;
  }
  if (is_connected != was_connected) {
    for (const auto& listener : connection_listeners_) {
      listener.Run();
    }
  }
  ScheduleNextStatePoll();
}

std::unique_ptr<NetworkClient> NetworkClient::CreateInstance(
    const std::set<std::string>& device_whitelist) {
  return std::unique_ptr<NetworkClient>{
      new BrilloNetworkClient{device_whitelist}};
}

}  // namespace buffet
