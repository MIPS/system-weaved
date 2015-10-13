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

#include "command.h"

#include "buffet/dbus-proxies.h"

namespace weaved {

Command::Command(com::android::Weave::CommandProxy* proxy) : proxy_{proxy} {}

const std::string& Command::GetID() const {
  return proxy_->id();
}

const std::string& Command::GetName() const {
  return proxy_->name();
}

Command::State Command::GetState() const {
  std::string state = proxy_->state();
  if (state == "queued")
    return Command::State::kQueued;
  else if (state == "inProgress")
    return Command::State::kInProgress;
  else if (state == "paused")
    return Command::State::kPaused;
  else if (state == "error")
    return Command::State::kError;
  else if (state == "done")
    return Command::State::kDone;
  else if (state == "cancelled")
    return Command::State::kCancelled;
  else if (state == "aborted")
    return Command::State::kAborted;
  else if (state == "expired")
    return Command::State::kExpired;
  LOG(WARNING) << "Unknown command state: " << state;
  return Command::State::kQueued;
}

Command::Origin Command::GetOrigin() const {
  std::string origin = proxy_->origin();
  if (origin == "local")
    return Command::Origin::kLocal;
  else if (origin == "cloud")
    return Command::Origin::kCloud;
  LOG(WARNING) << "Unknown command origin: " << origin;
  return Command::Origin::kLocal;
}

const brillo::VariantDictionary& Command::GetParameters() const {
  return proxy_->parameters();
}

bool Command::SetProgress(const brillo::VariantDictionary& progress,
                          brillo::ErrorPtr* error) {
  return proxy_->SetProgress(progress, error);
}

bool Command::Complete(const brillo::VariantDictionary& results,
                       brillo::ErrorPtr* error) {
  return proxy_->Complete(results, error);
}

bool Command::Abort(const std::string& error_code,
                    const std::string& error_message,
                    brillo::ErrorPtr* error) {
  return proxy_->Abort(error_code, error_message, error);
}

bool Command::Cancel(brillo::ErrorPtr* error) {
  return proxy_->Cancel(error);
}

}  // namespace weave
