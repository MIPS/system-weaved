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

#ifndef LIBWEAVED_COMMAND_H_
#define LIBWEAVED_COMMAND_H_

#include <string>

#include <base/macros.h>
#include <chromeos/errors/error.h>
#include <chromeos/variant_dictionary.h>
#include <libweaved/export.h>

namespace com {
namespace android {
namespace Weave {
class CommandProxy;
}  // namespace Weave
}  // namespace android
}  // namespace com


namespace weaved {

class Device;

class LIBWEAVED_EXPORT Command final {
 public:
  enum class State {
    kQueued,
    kInProgress,
    kPaused,
    kError,
    kDone,
    kCancelled,
    kAborted,
    kExpired,
  };

  enum class Origin { kLocal, kCloud };

  // Returns the full command ID.
  const std::string& GetID() const;

  // Returns the full name of the command.
  const std::string& GetName() const;

  // Returns the command state.
  Command::State GetState() const;

  // Returns the origin of the command.
  Command::Origin GetOrigin() const;

  // Returns the command parameters.
  const chromeos::VariantDictionary& GetParameters() const;

  // Helper function to get a command parameter of particular type T from the
  // command parameter list. Returns default value for type T (e.g. 0 for int or
  // or "" for std::string) if the parameter with the given name is not found or
  // is of incorrect type.
  template <typename T>
  T GetParameter(const std::string& name) const {
    const chromeos::VariantDictionary& parameters = GetParameters();
    T value{};
    auto p = parameters.find(name);
    if (p != parameters.end())
      p->second.GetValue<T>(&value);
    return value;
  }

  // Updates the command progress. The |progress| should match the schema.
  // Returns false if |progress| value is incorrect.
  bool SetProgress(const chromeos::VariantDictionary& progress,
                   chromeos::ErrorPtr* error);

  // Sets command into terminal "done" state.
  // Updates the command results. The |results| should match the schema.
  // Returns false if |results| value is incorrect.
  bool Complete(const chromeos::VariantDictionary& results,
                chromeos::ErrorPtr* error);

  // Aborts command execution.
  // Sets command into terminal "aborted" state.
  bool Abort(const std::string& error_code,
             const std::string& error_message,
             chromeos::ErrorPtr* error);

  // Cancels command execution.
  // Sets command into terminal "canceled" state.
  bool Cancel(chromeos::ErrorPtr* error);

 protected:
  Command(com::android::Weave::CommandProxy* proxy);

 private:
  friend class Device;
  com::android::Weave::CommandProxy* proxy_{nullptr};

  DISALLOW_COPY_AND_ASSIGN(Command);
};

}  // namespace weave

#endif  // LIBWEAVED_COMMAND_H_
