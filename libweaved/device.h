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


#ifndef LIBWEAVED_DEVICE_H_
#define LIBWEAVED_DEVICE_H_

#include <map>
#include <memory>
#include <string>

#include <base/callback.h>
#include <base/macros.h>
#include <base/memory/ref_counted.h>
#include <brillo/any.h>
#include <brillo/errors/error.h>
#include <brillo/variant_dictionary.h>
#include <libweaved/command.h>
#include <libweaved/export.h>

namespace com {
namespace android {
namespace Weave {
class CommandProxy;
class ManagerProxy;
class ObjectManagerProxy;
}  // namespace Weave
}  // namespace android
}  // namespace com

namespace dbus {
class Bus;
class ObjectPath;
}  // namespace dbus

namespace weaved {

class LIBWEAVED_EXPORT Device final {
 public:
  ~Device();

  static std::unique_ptr<Device> CreateInstance(
      const scoped_refptr<dbus::Bus>& bus,
      const base::Closure& state_required_callback);

  // Callback type for AddCommandHandler.
  using CommandHandlerCallback =
      base::Callback<void(const std::weak_ptr<Command>& command)>;

  // Sets handler for new commands added to the queue.
  // |command_name| is the full command name of the command to handle. e.g.
  // "base.reboot". Each command can have no more than one handler.
  // Empty |command_name| sets default handler for all unhanded commands.
  // No new command handlers can be set after default handler was set.
  void AddCommandHandler(const std::string& command_name,
                         const CommandHandlerCallback& callback);

  bool SetStateProperties(const brillo::VariantDictionary& dict,
                          brillo::ErrorPtr* error);

  // Sets value of the single property.
  // |name| is full property name, including package name. e.g. "base.network".
  bool SetStateProperty(const std::string& name,
                        const brillo::Any& value,
                        brillo::ErrorPtr* error);

 private:
  Device(const scoped_refptr<dbus::Bus>& bus,
         const base::Closure& state_required_callback);

  void OnCommandAdded(com::android::Weave::CommandProxy* proxy);
  void OnCommandRemoved(const dbus::ObjectPath& object_path);

  void OnManagerAdded(com::android::Weave::ManagerProxy* proxy);
  void OnManagerRemoved(const dbus::ObjectPath& object_path);

  std::unique_ptr<com::android::Weave::ObjectManagerProxy> weaved_object_mgr_;
  com::android::Weave::ManagerProxy* proxy_{nullptr};

  using CommandMap =
      std::map<com::android::Weave::CommandProxy*, std::shared_ptr<Command>>;
  CommandMap command_map_;
  std::map<std::string, CommandHandlerCallback> command_handler_map_;
  scoped_refptr<dbus::Bus> bus_;
  base::Closure state_required_callback_;


  DISALLOW_COPY_AND_ASSIGN(Device);
};

}  // namespace weave

#endif  // LIBWEAVE_INCLUDE_WEAVE_DEVICE_H_
