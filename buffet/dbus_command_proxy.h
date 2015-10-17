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

#ifndef BUFFET_DBUS_COMMAND_PROXY_H_
#define BUFFET_DBUS_COMMAND_PROXY_H_

#include <string>

#include <base/macros.h>
#include <base/scoped_observer.h>
#include <brillo/dbus/data_serialization.h>
#include <brillo/dbus/dbus_object.h>
#include <weave/command.h>

#include "buffet/dbus_bindings/com.android.Weave.Command.h"

namespace brillo {
namespace dbus_utils {
class ExportedObjectManager;
}  // namespace dbus_utils
}  // namespace brillo

namespace buffet {

class DBusCommandProxy : public com::android::Weave::CommandInterface {
 public:
  DBusCommandProxy(brillo::dbus_utils::ExportedObjectManager* object_manager,
                   const scoped_refptr<dbus::Bus>& bus,
                   const std::weak_ptr<weave::Command>& command,
                   std::string object_path);
  ~DBusCommandProxy() override = default;

  void RegisterAsync(
      const brillo::dbus_utils::AsyncEventSequencer::CompletionAction&
          completion_callback);

 private:
  bool SetProgress(brillo::ErrorPtr* error,
                   const brillo::VariantDictionary& progress) override;
  bool Complete(brillo::ErrorPtr* error,
                const brillo::VariantDictionary& results) override;
  bool Abort(brillo::ErrorPtr* error,
             const std::string& code,
             const std::string& message) override;
  bool Cancel(brillo::ErrorPtr* error) override;

  std::weak_ptr<weave::Command> command_;
  com::android::Weave::CommandAdaptor dbus_adaptor_{this};
  brillo::dbus_utils::DBusObject dbus_object_;

  friend class DBusCommandProxyTest;
  friend class DBusCommandDispacherTest;
  DISALLOW_COPY_AND_ASSIGN(DBusCommandProxy);
};

}  // namespace buffet

#endif  // BUFFET_DBUS_COMMAND_PROXY_H_
