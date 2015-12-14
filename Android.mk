# Copyright (C) 2015 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

LOCAL_PATH := $(call my-dir)

# Common variables
# ========================================================

buffetCommonCppExtension := .cc
buffetCommonCFlags := -DBUFFET_USE_WIFI_BOOTSTRAPPING -Wall -Werror \
	-Wno-char-subscripts -Wno-missing-field-initializers \
	-Wno-unused-function -Wno-unused-parameter \

buffetCommonCppFlags := \
	-Wno-deprecated-register \
	-Wno-sign-compare \
	-Wno-sign-promo \
	-Wno-non-virtual-dtor \

buffetCommonCIncludes := \
	$(LOCAL_PATH)/.. \
	$(LOCAL_PATH)/dbus-proxies \
	external/cros/system_api \
	external/gtest/include \

buffetSharedLibraries := \
	libapmanager-client \
	libavahi-common \
	libavahi-client \
	libbrillo \
	libbrillo-dbus \
	libbrillo-http \
	libbrillo-stream \
	libchrome \
	libchrome-dbus \
	libcutils \
	libdbus \
	libshill-client \
	libweave \
	libwebserv \

ifdef BRILLO

buffetSharedLibraries += \
	libkeymaster_messages \
	libkeystore_binder \

endif

# buffet-common
# ========================================================
include $(CLEAR_VARS)
LOCAL_MODULE := buffet-common
LOCAL_CPP_EXTENSION := $(buffetCommonCppExtension)
LOCAL_CFLAGS := $(buffetCommonCFlags)
# TODO(avakulenko): Remove -Wno-deprecated-declarations when legacy libweave
# APIs are removed (see: b/25917708).
LOCAL_CPPFLAGS := $(buffetCommonCppFlags) -Wno-deprecated-declarations
LOCAL_C_INCLUDES := $(buffetCommonCIncludes)
LOCAL_SHARED_LIBRARIES := $(buffetSharedLibraries)
LOCAL_STATIC_LIBRARIES :=
LOCAL_RTTI_FLAG := -frtti
LOCAL_CLANG := true
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)

LOCAL_SRC_FILES := \
	buffet/ap_manager_client.cc \
	buffet/avahi_mdns_client.cc \
	buffet/buffet_config.cc \
	buffet/dbus_command_dispatcher.cc \
	buffet/dbus_command_proxy.cc \
	buffet/dbus_conversion.cc \
	buffet/dbus_constants.cc \
	buffet/flouride_socket_bluetooth_client.cc \
	buffet/http_transport_client.cc \
	buffet/manager.cc \
	buffet/shill_client.cc \
	buffet/socket_stream.cc \
	buffet/webserv_client.cc \
	buffet/dbus_bindings/dbus-service-config.json \
	buffet/dbus_bindings/com.android.Weave.Command.dbus-xml \
	buffet/dbus_bindings/com.android.Weave.Manager.dbus-xml \

ifdef BRILLO
LOCAL_SRC_FILES += buffet/keystore_encryptor.cc
else
LOCAL_SRC_FILES += buffet/fake_encryptor.cc
endif

include $(BUILD_STATIC_LIBRARY)

# weaved-brillo-api
# ========================================================
include $(CLEAR_VARS)
LOCAL_MODULE := weaved-brillo-api
LOCAL_CPP_EXTENSION := $(buffetCommonCppExtension)
LOCAL_CFLAGS := $(buffetCommonCFlags)
LOCAL_CPPFLAGS := $(buffetCommonCppFlags)
LOCAL_CLANG := true
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)

LOCAL_SRC_FILES := \
	brillo/weaved_system_properties.cc \

include $(BUILD_STATIC_LIBRARY)

# weaved
# ========================================================
include $(CLEAR_VARS)
LOCAL_MODULE := weaved
LOCAL_REQUIRED_MODULES := \
	avahi-daemon \
	com.android.Weave.conf \
	libweaved \
	webservd \

LOCAL_CPP_EXTENSION := $(buffetCommonCppExtension)
LOCAL_CFLAGS := $(buffetCommonCFlags)
LOCAL_CPPFLAGS := $(buffetCommonCppFlags)
LOCAL_C_INCLUDES := $(buffetCommonCIncludes)
LOCAL_INIT_RC := weaved.rc
LOCAL_SHARED_LIBRARIES := $(buffetSharedLibraries)
LOCAL_STATIC_LIBRARIES := weaved-brillo-api
LOCAL_WHOLE_STATIC_LIBRARIES := buffet-common
LOCAL_CLANG := true
LOCAL_RTTI_FLAG := -frtti

LOCAL_SRC_FILES := \
	buffet/main.cc

include $(BUILD_EXECUTABLE)

# libweaved-internal
# ========================================================
# You do not want to depend on this.  Depend on libweaved instead.
# libweaved abstracts and helps you consume this interface.
include $(CLEAR_VARS)
LOCAL_MODULE := libweaved-internal
LOCAL_DBUS_PROXY_PREFIX := buffet

LOCAL_SRC_FILES := \
	buffet/dbus_bindings/dbus-service-config.json \
	buffet/dbus_bindings/com.android.Weave.Command.dbus-xml \
	buffet/dbus_bindings/com.android.Weave.Manager.dbus-xml \

include $(BUILD_SHARED_LIBRARY)

# libweaved
# ========================================================
include $(CLEAR_VARS)
LOCAL_MODULE := libweaved
LOCAL_CPP_EXTENSION := $(buffetCommonCppExtension)
LOCAL_CFLAGS := $(buffetCommonCFlags)
LOCAL_CPPFLAGS := $(buffetCommonCppFlags)
LOCAL_C_INCLUDES := external/gtest/include
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)
LOCAL_SHARED_LIBRARIES := \
	$(buffetSharedLibraries) \
	libweaved-internal \

LOCAL_STATIC_LIBRARIES :=
LOCAL_RTTI_FLAG := -frtti
LOCAL_CLANG := true

LOCAL_SRC_FILES := \
	libweaved/command.cc \
	libweaved/device.cc \

include $(BUILD_SHARED_LIBRARY)

# weaved_test
# ========================================================
include $(CLEAR_VARS)
LOCAL_MODULE := weaved_test
LOCAL_MODULE_TAGS := debug
LOCAL_CPP_EXTENSION := $(buffetCommonCppExtension)
LOCAL_CFLAGS := $(buffetCommonCFlags)
LOCAL_CPPFLAGS := $(buffetCommonCppFlags)
LOCAL_C_INCLUDES := \
	$(buffetCommonCIncludes) \
	external/gmock/include \

LOCAL_SHARED_LIBRARIES := \
	$(buffetSharedLibraries) \

LOCAL_STATIC_LIBRARIES := \
	buffet-common \
	libbrillo-test-helpers \
	libchrome_dbus_test_helpers \
	libchrome_test_helpers \
	libgtest \
	libgmock \
	libweave-test \
	weaved-brillo-api \

LOCAL_RTTI_FLAG := -frtti
LOCAL_CLANG := true

LOCAL_SRC_FILES := \
	buffet/buffet_config_unittest.cc \
	buffet/buffet_testrunner.cc \
	buffet/dbus_command_proxy_unittest.cc \
	buffet/dbus_conversion_unittest.cc \

include $(BUILD_NATIVE_TEST)

# DBus config files for /etc/dbus-1
# ========================================================
include $(CLEAR_VARS)
LOCAL_MODULE := com.android.Weave.conf
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT_ETC)/dbus-1
LOCAL_SRC_FILES := buffet/etc/dbus-1/com.android.Weave.conf
include $(BUILD_PREBUILT)
