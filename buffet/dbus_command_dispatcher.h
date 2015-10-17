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

#ifndef BUFFET_DBUS_COMMAND_DISPATCHER_H_
#define BUFFET_DBUS_COMMAND_DISPATCHER_H_

#include <map>
#include <string>

#include <base/macros.h>
#include <base/memory/weak_ptr.h>

namespace weave {
class Command;
class Device;
}

namespace brillo {
namespace dbus_utils {
class ExportedObjectManager;
}  // namespace dbus_utils
}  // namespace brillo

namespace buffet {

// Implements D-Bus dispatch of commands. When OnCommandAdded is called,
// DBusCommandDispacher creates an instance of DBusCommandProxy object and
// advertises it through ExportedObjectManager on D-Bus. Command handling
// processes can watch the new D-Bus object appear and communicate with it to
// update the command handling progress. Once command is handled,
// DBusCommandProxy::Done() is called and the command is removed from the
// command queue and D-Bus ExportedObjectManager.
class DBusCommandDispacher final {
 public:
  explicit DBusCommandDispacher(
      const base::WeakPtr<brillo::dbus_utils::ExportedObjectManager>&
          object_manager,
      weave::Device* device);

 private:
  void OnCommandAdded(const std::weak_ptr<weave::Command>& cmd);

  base::WeakPtr<brillo::dbus_utils::ExportedObjectManager> object_manager_;
  int next_id_{0};

  // Default constructor is used in special circumstances such as for testing.
  DBusCommandDispacher() = default;

  base::WeakPtrFactory<DBusCommandDispacher> weak_ptr_factory_{this};

  DISALLOW_COPY_AND_ASSIGN(DBusCommandDispacher);
};

}  // namespace buffet

#endif  // BUFFET_DBUS_COMMAND_DISPATCHER_H_
