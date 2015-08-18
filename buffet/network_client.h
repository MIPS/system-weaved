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

#ifndef BUFFET_NETWORK_CLIENT_H_
#define BUFFET_NETWORK_CLIENT_H_

#include <memory>
#include <set>
#include <string>

#include <base/cancelable_callback.h>
#include <weave/network.h>

#include "buffet/socket_stream.h"

namespace buffet {

class NetworkClient : public weave::Network {
 public:
  explicit NetworkClient(const std::set<std::string>& device_whitelist)
      : device_whitelist_{device_whitelist} {
  }

  ~NetworkClient() override = default;

  // Implements the Network interface.
  void AddOnConnectionChangedCallback(
      const OnConnectionChangedCallback& listener) override {
  }

  bool ConnectToService(const std::string& ssid,
                        const std::string& passphrase,
                        const base::Closure& on_success,
                        weave::ErrorPtr* error) override {
    return true;
  }

  weave::NetworkState GetConnectionState() const override {
    return weave::NetworkState::kOffline;
  }

  void EnableAccessPoint(const std::string& ssid) override {
  }

  void DisableAccessPoint() override {
  }

  std::unique_ptr<weave::Stream> OpenSocketBlocking(const std::string& host,
                                                    uint16_t port) override {
    return SocketStream::ConnectBlocking(host, port);
  }

  void CreateTlsStream(
      std::unique_ptr<weave::Stream> socket,
      const std::string& host,
      const base::Callback<void(std::unique_ptr<weave::Stream>)>& on_success,
      const base::Callback<void(const weave::Error*)>& on_error) override {
    SocketStream::TlsConnect(std::move(socket), host, on_success, on_error);
  }

  static std::unique_ptr<NetworkClient> CreateInstance(
      const std::set<std::string>& device_whitelist);

 protected:
  std::set<std::string> device_whitelist_;

 private:
  DISALLOW_COPY_AND_ASSIGN(NetworkClient);
};

}  // namespace buffet

#endif  // BUFFET_NETWORK_CLIENT_H_

