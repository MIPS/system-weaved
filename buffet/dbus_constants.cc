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

#include "buffet/dbus_constants.h"

namespace buffet {

namespace dbus_constants {

const char kServiceName[] = "com.android.Weave";
const char kRootServicePath[] = "/com/android/Weave";
const char kCommandServicePathPrefix[] = "/com/android/Weave/commands/";

namespace avahi {

const char kServiceName[] = "org.freedesktop.Avahi";

const char kServerInterface[] = "org.freedesktop.Avahi.Server";
const char kServerPath[] = "/";
const char kServerMethodEntryGroupNew[] = "EntryGroupNew";
const char kServerMethodServiceBrowserNew[] = "ServiceBrowserNew";
const char kServerMethodServiceResolverNew[] = "ServiceResolverNew";
const char kServerMethodGetHostName[] = "GetHostName";
const char kServerMethodGetState[] = "GetState";
const char kServerSignalStateChanged[] = "StateChanged";

const char kGroupInterface[] = "org.freedesktop.Avahi.EntryGroup";
const char kGroupMethodAddRecord[] = "AddRecord";
const char kGroupMethodAddService[] = "AddService";
const char kGroupMethodCommit[] = "Commit";
const char kGroupMethodFree[] = "Free";
const char kGroupMethodUpdateServiceTxt[] = "UpdateServiceTxt";
const char kGroupMethodReset[]= "Reset";
const char kGroupSignalStateChanged[] = "StateChanged";

const char kServiceBrowserInterface[] = "org.freedesktop.Avahi.ServiceBrowser";
const char kServiceBrowserMethodFree[] = "Free";
const char kServiceBrowserSignalItemNew[] = "ItemNew";
const char kServiceBrowserSignalItemRemove[] = "ItemRemove";
const char kServiceBrowserSignalFailure[] = "Failure";

const char kServiceResolverInterface[] =
    "org.freedesktop.Avahi.ServiceResolver";
const char kServiceResolverMethodFree[] = "Free";
const char kServiceResolverSignalFound[] = "Found";
const char kServiceResolverSignalFailure[] = "Failure";

}  // namespace avahi

}  // namespace dbus constants

}  // namespace buffet
