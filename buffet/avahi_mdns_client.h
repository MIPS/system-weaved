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

#include <weave/mdns.h>

#include "buffet/mdns_client.h"

namespace buffet {

// Publishes privet service on mDns using Avahi.
class AvahiMdnsClient : public MdnsClient {
 public:
  AvahiMdnsClient();
  ~AvahiMdnsClient() override;

  // weave::Mdns implementation.
  void PublishService(const std::string& service_name,
                      uint16_t port,
                      const std::map<std::string, std::string>& txt) override;
  void StopPublishing(const std::string& service_name) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(AvahiMdnsClient);
};

}  // namespace buffet

#endif  // BUFFET_AVAHI_MDNS_CLIENT_H_
