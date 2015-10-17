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

#include "buffet/dbus_command_dispatcher.h"

#include <brillo/dbus/exported_object_manager.h>
#include <weave/command.h>
#include <weave/device.h>

#include "buffet/dbus_command_proxy.h"
#include "buffet/dbus_constants.h"

using brillo::dbus_utils::AsyncEventSequencer;
using brillo::dbus_utils::ExportedObjectManager;

namespace buffet {

DBusCommandDispacher::DBusCommandDispacher(
    const base::WeakPtr<ExportedObjectManager>& object_manager,
    weave::Device* device)
    : object_manager_{object_manager} {
  device->AddCommandHandler("",
                            base::Bind(&DBusCommandDispacher::OnCommandAdded,
                                       weak_ptr_factory_.GetWeakPtr()));
}

void DBusCommandDispacher::OnCommandAdded(
    const std::weak_ptr<weave::Command>& cmd) {
  auto command = cmd.lock();
  if (!object_manager_ || !command)
    return;
  std::unique_ptr<DBusCommandProxy> proxy{new DBusCommandProxy(
      object_manager_.get(), object_manager_->GetBus(), command,
      buffet::dbus_constants::kCommandServicePathPrefix +
          std::to_string(++next_id_))};
  proxy->RegisterAsync(AsyncEventSequencer::GetDefaultCompletionAction());
  // DBusCommandProxy::DBusCommandProxy() subscribe itself to weave::Command
  // notifications. When weave::Command is being destroyed it sends
  // ::OnCommandDestroyed() and DBusCommandProxy deletes itself.
  proxy.release();
}

}  // namespace buffet
