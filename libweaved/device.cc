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

#include "device.h"

#include "buffet/dbus-proxies.h"

using com::android::Weave::CommandProxy;
using com::android::Weave::ManagerProxy;

namespace weaved {

Device::Device(const scoped_refptr<dbus::Bus>& bus,
               const base::Closure& state_required_callback)
    : bus_{bus}, state_required_callback_{state_required_callback} {
  weaved_object_mgr_.reset(new com::android::Weave::ObjectManagerProxy{bus_});
  weaved_object_mgr_->SetCommandAddedCallback(
      base::Bind(&Device::OnCommandAdded, base::Unretained(this)));
  weaved_object_mgr_->SetCommandRemovedCallback(
      base::Bind(&Device::OnCommandRemoved, base::Unretained(this)));
  weaved_object_mgr_->SetManagerAddedCallback(
      base::Bind(&Device::OnManagerAdded, base::Unretained(this)));
  weaved_object_mgr_->SetManagerRemovedCallback(
      base::Bind(&Device::OnManagerRemoved, base::Unretained(this)));
}

Device::~Device() {
}

std::unique_ptr<Device> Device::CreateInstance(
    const scoped_refptr<dbus::Bus>& bus,
    const base::Closure& state_required_callback) {
  return std::unique_ptr<Device>{new Device{bus, state_required_callback}};
}

void Device::AddCommandHandler(const std::string& command_name,
                               const CommandHandlerCallback& callback) {
  command_handler_map_.emplace(command_name, callback);

  // If there are any commands already received, call the handler immediately.
  for (auto& pair : command_map_) {
    if (pair.first->name() == command_name) {
      if (!pair.second)
        pair.second.reset(new Command{pair.first});
      callback.Run(pair.second);
    }
  }
}

bool Device::SetStateProperties(const brillo::VariantDictionary& dict,
                                brillo::ErrorPtr* error) {
  if (proxy_)
    return proxy_->UpdateState(dict, error);

  brillo::Error::AddTo(error, FROM_HERE, "weaved", "service_unavailable",
                         "Process 'weaved' is unreachable");
  return false;
}

bool Device::SetStateProperty(const std::string& name,
                              const brillo::Any& value,
                              brillo::ErrorPtr* error) {
  return SetStateProperties(brillo::VariantDictionary{{name, value}}, error);
}

void Device::OnCommandAdded(CommandProxy* proxy) {
  std::shared_ptr<Command>& command = command_map_[proxy];
  auto iter = command_handler_map_.find(proxy->name());
  if (iter == command_handler_map_.end())
    return;
  if (!command)
    command.reset(new Command{proxy});
  iter->second.Run(command);
}

void Device::OnCommandRemoved(const dbus::ObjectPath& object_path) {
  auto proxy = weaved_object_mgr_->GetCommandProxy(object_path);
  if (!proxy)
    return;
  command_map_.erase(proxy);
}

void Device::OnManagerAdded(ManagerProxy* proxy) {
  proxy_ = proxy;
  state_required_callback_.Run();
}

void Device::OnManagerRemoved(const dbus::ObjectPath& object_path) {
  proxy_ = nullptr;
}

}  // namespace weave
