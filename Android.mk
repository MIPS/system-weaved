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

# TODO(avakulenko): Remove this condition when libchromeos can be built on
# non-Linux host.
ifeq ($(HOST_OS),linux)

# Common variables
# ========================================================

buffetCommonCppExtension := .cc
buffetCommonCFlags := -D__BRILLO__ -Wall -Werror \
	-Wno-char-subscripts -Wno-missing-field-initializers \
	-Wno-unused-function -Wno-unused-parameter

buffetCommonCppFlags := \
	-Wno-deprecated-register \
	-Wno-sign-compare \
	-Wno-sign-promo \
	-Wno-non-virtual-dtor \

buffetCommonCIncludes := \
	$(LOCAL_PATH)/.. \
	$(LOCAL_PATH)/dbus-proxies \
	external/gtest/include \

buffetSharedLibraries := \
	libchrome \
	libchrome-dbus \
	libchromeos \
	libchromeos-dbus \
	libchromeos-http \
	libchromeos-stream \
	libdbus \
	libweave \

#	libconnectivity \

# buffet-common
# ========================================================
include $(CLEAR_VARS)
LOCAL_MODULE := buffet-common
LOCAL_CPP_EXTENSION := $(buffetCommonCppExtension)
LOCAL_CFLAGS := $(buffetCommonCFlags)
LOCAL_CPPFLAGS := $(buffetCommonCppFlags)
LOCAL_C_INCLUDES := $(buffetCommonCIncludes)
LOCAL_SHARED_LIBRARIES := $(buffetSharedLibraries)
LOCAL_STATIC_LIBRARIES :=
LOCAL_RTTI_FLAG := -frtti
LOCAL_CLANG := true
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)

LOCAL_SRC_FILES := \
	buffet/dbus_command_dispatcher.cc \
	buffet/dbus_command_proxy.cc \
	buffet/dbus_conversion.cc \
	buffet/dbus_constants.cc \
	buffet/http_transport_client.cc \
	buffet/manager.cc \
	buffet/mdns_client.cc \
	buffet/network_client.cc \

#	buffet/dbus_bindings/org.chromium.Buffet.Command.xml \
#	buffet/dbus_bindings/org.chromium.Buffet.Manager.xml \
#	buffet/ap_manager_client.cc \
#	buffet/peerd_client.cc \
#	buffet/webserv_client.cc \

include $(BUILD_STATIC_LIBRARY)

# buffet
# ========================================================
include $(CLEAR_VARS)
LOCAL_MODULE := weaved
LOCAL_REQUIRED_MODULES := \
	base_state.defaults.json \
	base_state.schema.json \
	gcd.json \
	org.chromium.Buffet.conf \

ifdef INITRC_TEMPLATE
LOCAL_REQUIRED_MODULES += init.weaved.rc
endif

LOCAL_CPP_EXTENSION := $(buffetCommonCppExtension)
LOCAL_CFLAGS := $(buffetCommonCFlags)
LOCAL_CPPFLAGS := $(buffetCommonCppFlags)
LOCAL_C_INCLUDES := $(buffetCommonCIncludes)
LOCAL_SHARED_LIBRARIES := $(buffetSharedLibraries)
LOCAL_WHOLE_STATIC_LIBRARIES := buffet-common
LOCAL_CLANG := true
LOCAL_RTTI_FLAG := -frtti

LOCAL_SRC_FILES := \
	buffet/main.cc

include $(BUILD_EXECUTABLE)

ifdef INITRC_TEMPLATE
include $(CLEAR_VARS)
LOCAL_MODULE := init.weaved.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(PRODUCT_OUT)/$(TARGET_COPY_OUT_INITRCD)

include $(BUILD_SYSTEM)/base_rules.mk

.PHONY: $(LOCAL_BUILT_MODULE)
$(LOCAL_BUILT_MODULE): $(INITRC_TEMPLATE)
	$(call generate-initrc-file,weaved,,)
endif

# buffet_testrunner
# ========================================================
include $(CLEAR_VARS)
LOCAL_MODULE := buffet_testrunner
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
	libchrome_dbus_test_helpers \
	libchrome_test_helpers \
	libchromeos-test-helpers \
	libgtest \
	libgmock \
	libweave-test \

LOCAL_RTTI_FLAG := -frtti
LOCAL_CLANG := true

LOCAL_SRC_FILES := \
	buffet/buffet_testrunner.cc \
	buffet/dbus_command_proxy_unittest.cc \
	buffet/dbus_conversion_unittest.cc \

include $(BUILD_NATIVE_TEST)

# Config files for /etc/buffet
# ========================================================
include $(CLEAR_VARS)
LOCAL_MODULE := base_state.defaults.json
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT_ETC)/buffet
LOCAL_SRC_FILES := buffet/etc/buffet/base_state.defaults.json
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := base_state.schema.json
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT_ETC)/buffet
LOCAL_SRC_FILES := buffet/etc/buffet/base_state.schema.json
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := gcd.json
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT_ETC)/buffet
LOCAL_SRC_FILES := buffet/etc/buffet/gcd.json
include $(BUILD_PREBUILT)

# DBus config files for /etc/dbus-1
# ========================================================
include $(CLEAR_VARS)
LOCAL_MODULE := org.chromium.Buffet.conf
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT_ETC)/dbus-1
LOCAL_SRC_FILES := buffet/etc/dbus-1/org.chromium.Buffet.conf
include $(BUILD_PREBUILT)

endif # HOST_OS == linux
