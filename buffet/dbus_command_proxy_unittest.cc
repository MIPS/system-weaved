// Copyright 2014 The Chromium OS Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "buffet/dbus_command_proxy.h"

#include <functional>
#include <memory>
#include <vector>

#include <dbus/mock_bus.h>
#include <dbus/mock_exported_object.h>
#include <dbus/property.h>
#include <chromeos/dbus/dbus_object.h>
#include <chromeos/dbus/dbus_object_test_helpers.h>
#include <gtest/gtest.h>
#include <weave/command.h>
#include <weave/enum_to_string.h>
#include <weave/test/mock_command.h>
#include <weave/test/unittest_utils.h>

#include "buffet/dbus_constants.h"

namespace buffet {

using ::testing::_;
using ::testing::AnyNumber;
using ::testing::Return;
using ::testing::ReturnRefOfCopy;
using ::testing::StrictMock;

using chromeos::VariantDictionary;
using chromeos::dbus_utils::AsyncEventSequencer;
using weave::test::CreateDictionaryValue;
using weave::test::IsEqualValue;

namespace {

const char kTestCommandId[] = "cmd_1";

MATCHER_P(EqualToJson, json, "") {
  auto json_value = CreateDictionaryValue(json);
  return IsEqualValue(*json_value, arg);
}

}  // namespace

class DBusCommandProxyTest : public ::testing::Test {
 public:
  void SetUp() override {
    command_ = std::make_shared<StrictMock<weave::test::MockCommand>>();
    // Set up a mock DBus bus object.
    dbus::Bus::Options options;
    options.bus_type = dbus::Bus::SYSTEM;
    bus_ = new dbus::MockBus(options);
    // By default, don't worry about threading assertions.
    EXPECT_CALL(*bus_, AssertOnOriginThread()).Times(AnyNumber());
    EXPECT_CALL(*bus_, AssertOnDBusThread()).Times(AnyNumber());

    EXPECT_CALL(*command_, GetID())
        .WillOnce(ReturnRefOfCopy<std::string>(kTestCommandId));
    // Use WillRepeatedly because GetName is used for logging.
    EXPECT_CALL(*command_, GetName())
        .WillRepeatedly(ReturnRefOfCopy<std::string>("robot.jump"));
    EXPECT_CALL(*command_, GetStatus())
        .WillRepeatedly(Return(weave::CommandStatus::kQueued));
    EXPECT_CALL(*command_, GetOrigin())
        .WillOnce(Return(weave::CommandOrigin::kLocal));
    EXPECT_CALL(*command_, MockGetParameters())
        .WillOnce(ReturnRefOfCopy<std::string>(R"({
          'height': 53,
          '_jumpType': '_withKick'
        })"));
    EXPECT_CALL(*command_, MockGetProgress())
        .WillRepeatedly(ReturnRefOfCopy<std::string>("{}"));
    EXPECT_CALL(*command_, MockGetResults())
        .WillOnce(ReturnRefOfCopy<std::string>("{}"));

    // Set up a mock ExportedObject to be used with the DBus command proxy.
    std::string cmd_path = buffet::dbus_constants::kCommandServicePathPrefix;
    cmd_path += kTestCommandId;
    const dbus::ObjectPath kCmdObjPath(cmd_path);
    // Use a mock exported object for the exported object manager.
    mock_exported_object_command_ =
        new dbus::MockExportedObject(bus_.get(), kCmdObjPath);
    EXPECT_CALL(*bus_, GetExportedObject(kCmdObjPath))
        .Times(AnyNumber())
        .WillRepeatedly(Return(mock_exported_object_command_.get()));
    EXPECT_CALL(*mock_exported_object_command_, ExportMethod(_, _, _, _))
        .Times(AnyNumber());

    proxy_.reset(new DBusCommandProxy{
        nullptr, bus_, std::weak_ptr<weave::Command>{command_}, cmd_path});
    GetCommandProxy()->RegisterAsync(
        AsyncEventSequencer::GetDefaultCompletionAction());
  }

  void TearDown() override {
    EXPECT_CALL(*mock_exported_object_command_, Unregister()).Times(1);
    bus_ = nullptr;
  }

  DBusCommandProxy* GetCommandProxy() const { return proxy_.get(); }

  com::android::Weave::CommandAdaptor* GetCommandAdaptor() const {
    return &GetCommandProxy()->dbus_adaptor_;
  }

  com::android::Weave::CommandInterface* GetCommandInterface() const {
    // DBusCommandProxy also implements CommandInterface.
    return GetCommandProxy();
  }

  weave::CommandStatus GetCommandStatus() const {
    weave::CommandStatus status;
    EXPECT_TRUE(StringToEnum(GetCommandAdaptor()->GetStatus(), &status));
    return status;
  }

  scoped_refptr<dbus::MockExportedObject> mock_exported_object_command_;
  scoped_refptr<dbus::MockBus> bus_;

  std::shared_ptr<StrictMock<weave::test::MockCommand>> command_;
  std::unique_ptr<DBusCommandProxy> proxy_;
};

TEST_F(DBusCommandProxyTest, Init) {
  VariantDictionary params = {
      {"height", int32_t{53}}, {"_jumpType", std::string{"_withKick"}},
  };
  EXPECT_EQ(weave::CommandStatus::kQueued, GetCommandStatus());
  EXPECT_EQ(params, GetCommandAdaptor()->GetParameters());
  EXPECT_EQ(VariantDictionary{}, GetCommandAdaptor()->GetProgress());
  EXPECT_EQ(VariantDictionary{}, GetCommandAdaptor()->GetResults());
  EXPECT_EQ("robot.jump", GetCommandAdaptor()->GetName());
  EXPECT_EQ(kTestCommandId, GetCommandAdaptor()->GetId());
}

TEST_F(DBusCommandProxyTest, SetProgress) {
  EXPECT_CALL(*command_, SetProgress(EqualToJson("{'progress': 10}"), _))
      .WillOnce(Return(true));
  EXPECT_TRUE(
      GetCommandInterface()->SetProgress(nullptr, {{"progress", int32_t{10}}}));
}

TEST_F(DBusCommandProxyTest, SetResults) {
  EXPECT_CALL(
      *command_,
      SetResults(
          EqualToJson("{'foo': 42, 'bar': 'foobar', 'resultList': [1, 2, 3]}"),
          _))
      .WillOnce(Return(true));
  EXPECT_TRUE(GetCommandInterface()->SetResults(
      nullptr, VariantDictionary{{"foo", int32_t{42}},
                                 {"bar", std::string{"foobar"}},
                                 {"resultList", std::vector<int>{1, 2, 3}}}));
}

TEST_F(DBusCommandProxyTest, Abort) {
  EXPECT_CALL(*command_, Abort());
  EXPECT_TRUE(GetCommandInterface()->Abort(nullptr));
}

TEST_F(DBusCommandProxyTest, Cancel) {
  EXPECT_CALL(*command_, Cancel());
  EXPECT_TRUE(GetCommandInterface()->Cancel(nullptr));
}

TEST_F(DBusCommandProxyTest, Done) {
  EXPECT_CALL(*command_, Done());
  EXPECT_TRUE(GetCommandInterface()->Done(nullptr));
}

}  // namespace buffet
