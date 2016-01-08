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

#ifndef LIBWEAVED_COMMAND_H_
#define LIBWEAVED_COMMAND_H_

#include <string>

#include <base/macros.h>
#include <brillo/errors/error.h>
#include <brillo/variant_dictionary.h>
#include <libweaved/export.h>
#include <utils/StrongPointer.h>

namespace android {
namespace weave {
class IWeaveCommand;
}  // namespace weave
}  // namespace android

namespace weaved {

class ServiceImpl;

namespace detail {

// Helper function for Command::GetParameter<T>. Allows specialization.
template <typename T>
inline bool GetValue(const brillo::Any& any, T* value) {
  return any.GetValue<T>(value);
}

// Specialization for double, allow to extract a double from an int.
template <>
inline bool GetValue<double>(const brillo::Any& any, double* value) {
  if (any.GetValue<double>(value))
    return true;

  int int_val = 0;
  if (!any.GetValue<int>(&int_val))
    return false;

  *value = static_cast<double>(int_val);
  return true;
}

}  // namespace detail

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

  ~Command();

  // Returns the full command ID.
  std::string GetID() const;

  // Returns the full name of the command.
  std::string GetName() const;

  // Returns the name of the component this command was sent to.
  std::string GetComponent() const;

  // Returns the command state.
  Command::State GetState() const;

  // Returns the origin of the command.
  Command::Origin GetOrigin() const;

  // Returns the command parameters.
  brillo::VariantDictionary GetParameters() const;

  // Helper function to get a command parameter of particular type T from the
  // command parameter list. Returns default value for type T (e.g. 0 for int or
  // or "" for std::string) if the parameter with the given name is not found or
  // is of incorrect type.
  template <typename T>
  T GetParameter(const std::string& name) const {
    const brillo::VariantDictionary& parameters = GetParameters();
    T value{};
    auto p = parameters.find(name);
    if (p != parameters.end())
      detail::GetValue<T>(p->second, &value);
    return value;
  }

  // Updates the command progress. The |progress| should match the schema.
  // Returns false if |progress| value is incorrect.
  bool SetProgress(const brillo::VariantDictionary& progress,
                   brillo::ErrorPtr* error);

  // Sets command into terminal "done" state.
  // Updates the command results. The |results| should match the schema.
  // Returns false if |results| value is incorrect.
  bool Complete(const brillo::VariantDictionary& results,
                brillo::ErrorPtr* error);

  // Aborts command execution.
  // Sets command into terminal "aborted" state.
  bool Abort(const std::string& error_code,
             const std::string& error_message,
             brillo::ErrorPtr* error);

  // Cancels command execution.
  // Sets command into terminal "canceled" state.
  bool Cancel(brillo::ErrorPtr* error);

 protected:
  explicit Command(const android::sp<android::weave::IWeaveCommand>& proxy);

 private:
  friend class ServiceImpl;
  android::sp<android::weave::IWeaveCommand> binder_proxy_;

  DISALLOW_COPY_AND_ASSIGN(Command);
};

}  // namespace weave

#endif  // LIBWEAVED_COMMAND_H_
