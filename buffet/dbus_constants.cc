// Copyright 2014 The Chromium OS Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

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
