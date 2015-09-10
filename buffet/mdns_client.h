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

#ifndef BUFFET_MDNS_CLIENT_H_
#define BUFFET_MDNS_CLIENT_H_

#include <map>
#include <memory>
#include <string>

#include <base/guid.h>
#include <base/memory/ref_counted.h>
#include <dbus/bus.h>
#include <weave/mdns.h>

namespace buffet {

// Stub MDNS implementation that does nothing on platform without MDNS support.
class MdnsClient : public weave::Mdns {
 public:
  MdnsClient() : device_id_{base::GenerateGUID()} {}
  ~MdnsClient() override = default;

  // weave::Mdns implementation.
  void PublishService(const std::string& service_type, uint16_t port,
                      const std::vector<std::string>& txt) override {}
  void StopPublishing(const std::string& service_type) override {}
  std::string GetId() const override { return device_id_; }

  static std::unique_ptr<MdnsClient> CreateInstance(
      const scoped_refptr<dbus::Bus>& bus);

 protected:
  // Cached value of the device ID that we got from peerd.
  std::string device_id_;

  DISALLOW_COPY_AND_ASSIGN(MdnsClient);
};

}  // namespace buffet

#endif  // BUFFET_MDNS_CLIENT_H_
