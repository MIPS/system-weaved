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

#include "buffet/network_client.h"

#include <base/message_loop/message_loop.h>

namespace buffet {

namespace {
const char kErrorCommandFailed[] = "commandFailed";
const int kConnectionTimeoutSeconds = 30;
const int kConnectionActivePollSeconds = 3;
const int kConnectionInactivePollSeconds = 10;
}  // namespace

NetworkClient::NetworkClient()
    : state_(weave::NetworkState::kOffline) {}

NetworkClient::~NetworkClient() {}

void NetworkClient::AddOnConnectionChangedCallback(
    const OnConnectionChangedCallback& listener) {
  connection_listeners_.push_back(listener);
}

bool NetworkClient::ConnectToService(const std::string& ssid,
                                     const std::string& passphrase,
                                     const base::Closure& on_success,
                                     chromeos::ErrorPtr* error) {
  if (!connectivity_client_.ConnectToAccessPoint(ssid, passphrase)) {
    chromeos::Error::AddTo(error, FROM_HERE, kErrorCommandFailed, "", "");
    return false;
  }

  connection_success_closure_ = on_success;
  state_ = weave::NetworkState::kConnecting;

  connection_timeout_closure_.Reset(
      base::Bind(&NetworkClient::OnConnectionTimeout, base::Unretained(this)));
  base::MessageLoop::current()->PostDelayedTask(
      FROM_HERE,
      connection_timeout_closure_.callback(),
      base::TimeDelta::FromSeconds(kConnectionTimeoutSeconds));

  ScheduleNextStatePoll();

  return true;
}

weave::NetworkState NetworkClient::GetConnectionState() const {
  return state_;
}

void NetworkClient::EnableAccessPoint(const std::string& ssid) {
  connectivity_client_.EnableAccessPoint(ssid);
  state_ = weave::NetworkState::kOffline;
}

void NetworkClient::DisableAccessPoint() {
  connectivity_client_.DisableAccessPoint();
}

void NetworkClient::OnConnectionTimeout() {
  state_ = weave::NetworkState::kFailure;
}

void NetworkClient::ScheduleNextStatePoll() {
  periodic_connection_state_closure_.Reset(
      base::Bind(&NetworkClient::UpdateConnectionState,
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

void NetworkClient::UpdateConnectionState() {
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
      listener.Run(is_connected);
    }
  }
  ScheduleNextStatePoll();
}

}  // namespace buffet
