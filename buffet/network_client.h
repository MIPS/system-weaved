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
#include <chromeos/errors/error_codes.h>
#include <weave/network_provider.h>
#include <weave/wifi_provider.h>

#include "buffet/socket_stream.h"
#include "buffet/weave_error_conversion.h"

namespace buffet {

class NetworkClient : public weave::NetworkProvider,
                      public weave::WifiProvider {
 public:
  explicit NetworkClient(const std::set<std::string>& device_whitelist)
      : device_whitelist_{device_whitelist} {
  }

  ~NetworkClient() override = default;

  // NetworkProvider implementation.
  void AddConnectionChangedCallback(
      const ConnectionChangedCallback& listener) override {}

  weave::NetworkState GetConnectionState() const override {
    return weave::NetworkState::kOffline;
  }

  void OpenSslSocket(
      const std::string& host,
      uint16_t port,
      const base::Callback<void(std::unique_ptr<weave::Stream>)>& on_success,
      const weave::ErrorCallback& on_error) override {
    auto socket = SocketStream::ConnectBlocking(host, port);
    if (socket) {
      SocketStream::TlsConnect(std::move(socket), host, on_success, on_error);
      return;
    }
    chromeos::ErrorPtr error;
    chromeos::errors::system::AddSystemError(&error, FROM_HERE, errno);
    weave::ErrorPtr weave_error;
    ConvertError(*error.get(), &weave_error);
    on_error.Run(weave_error.get());
  }

  // WifiProvider implementation.
  void Connect(const std::string& ssid,
               const std::string& passphrase,
               const weave::SuccessCallback& on_success,
               const weave::ErrorCallback& on_error) override {}

  void StartAccessPoint(const std::string& ssid) override {}

  void StopAccessPoint() override {}

  static std::unique_ptr<NetworkClient> CreateInstance(
      const std::set<std::string>& device_whitelist);

 protected:
  std::set<std::string> device_whitelist_;

 private:
  DISALLOW_COPY_AND_ASSIGN(NetworkClient);
};

}  // namespace buffet

#endif  // BUFFET_NETWORK_CLIENT_H_

