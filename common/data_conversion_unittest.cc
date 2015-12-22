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

#include "common/data_conversion.h"

#include <limits>
#include <memory>
#include <string>
#include <vector>

#include <base/guid.h>
#include <base/rand_util.h>
#include <base/values.h>
#include <brillo/variant_dictionary.h>
#include <gtest/gtest.h>
#include <weave/test/unittest_utils.h>

namespace weaved {

namespace {

using brillo::Any;
using brillo::VariantDictionary;
using weave::test::CreateDictionaryValue;
using weave::test::IsEqualValue;

brillo::VariantDictionary ToVariant(const base::DictionaryValue& object) {
  return details::DictionaryValueToVariantDictionary(object);
}

std::unique_ptr<base::DictionaryValue> FromVariant(
    const brillo::VariantDictionary& object) {
  brillo::ErrorPtr error;
  auto result = details::VariantDictionaryToDictionaryValue(object, &error);
  EXPECT_TRUE(result || error);
  return result;
}

std::unique_ptr<base::Value> CreateRandomValue(int children);
std::unique_ptr<base::Value> CreateRandomValue(int children,
                                               base::Value::Type type);

const base::Value::Type kRandomTypes[] = {
    base::Value::TYPE_BOOLEAN,    base::Value::TYPE_INTEGER,
    base::Value::TYPE_DOUBLE,     base::Value::TYPE_STRING,
    base::Value::TYPE_DICTIONARY, base::Value::TYPE_LIST,
};

const base::Value::Type kRandomTypesWithChildren[] = {
    base::Value::TYPE_DICTIONARY, base::Value::TYPE_LIST,
};

base::Value::Type CreateRandomValueType(bool with_children) {
  if (with_children) {
    return kRandomTypesWithChildren[base::RandInt(
        0, arraysize(kRandomTypesWithChildren) - 1)];
  }
  return kRandomTypes[base::RandInt(0, arraysize(kRandomTypes) - 1)];
}

std::unique_ptr<base::DictionaryValue> CreateRandomDictionary(int children) {
  std::unique_ptr<base::DictionaryValue> result{new base::DictionaryValue};

  while (children > 0) {
    int sub_children = base::RandInt(1, children);
    children -= sub_children;
    result->Set(base::GenerateGUID(),
                CreateRandomValue(sub_children).release());
  }

  return result;
}

std::unique_ptr<base::ListValue> CreateRandomList(int children) {
  std::unique_ptr<base::ListValue> result{new base::ListValue};

  base::Value::Type type = CreateRandomValueType(children > 0);
  while (children > 0) {
    size_t max_children =
        (type != base::Value::TYPE_DICTIONARY && type != base::Value::TYPE_LIST)
            ? 1
            : children;
    size_t sub_children = base::RandInt(1, max_children);
    children -= sub_children;
    result->Append(CreateRandomValue(sub_children, type).release());
  }

  return result;
}

std::unique_ptr<base::Value> CreateRandomValue(int children,
                                               base::Value::Type type) {
  CHECK_GE(children, 1);
  switch (type) {
    case base::Value::TYPE_INTEGER:
      return std::unique_ptr<base::Value>{new base::FundamentalValue{
          base::RandInt(std::numeric_limits<int>::min(),
                        std::numeric_limits<int>::max())}};
    case base::Value::TYPE_DOUBLE:
      return std::unique_ptr<base::Value>{
          new base::FundamentalValue{base::RandDouble()}};
    case base::Value::TYPE_STRING:
      return std::unique_ptr<base::Value>{
          new base::StringValue{base::GenerateGUID()}};
    case base::Value::TYPE_DICTIONARY:
      CHECK_GE(children, 1);
      return CreateRandomDictionary(children - 1);
    case base::Value::TYPE_LIST:
      CHECK_GE(children, 1);
      return CreateRandomList(children - 1);
    default:
      return std::unique_ptr<base::Value>{
          new base::FundamentalValue{base::RandInt(0, 1) != 0}};
  }
}

std::unique_ptr<base::Value> CreateRandomValue(int children) {
  return CreateRandomValue(children, CreateRandomValueType(children > 0));
}

}  // namespace

TEST(DBusConversionTest, DictionaryToDBusVariantDictionary) {
  EXPECT_EQ((VariantDictionary{{"bool", true}}),
            ToVariant(*CreateDictionaryValue("{'bool': true}")));
  EXPECT_EQ((VariantDictionary{{"int", 5}}),
            ToVariant(*CreateDictionaryValue("{'int': 5}")));
  EXPECT_EQ((VariantDictionary{{"double", 6.7}}),
            ToVariant(*CreateDictionaryValue("{'double': 6.7}")));
  EXPECT_EQ((VariantDictionary{{"string", std::string{"abc"}}}),
            ToVariant(*CreateDictionaryValue("{'string': 'abc'}")));
  EXPECT_EQ((VariantDictionary{{"object", VariantDictionary{{"bool", true}}}}),
            ToVariant(*CreateDictionaryValue("{'object': {'bool': true}}")));
  EXPECT_EQ((VariantDictionary{{"emptyList", std::vector<Any>{}}}),
            ToVariant(*CreateDictionaryValue("{'emptyList': []}")));
  EXPECT_EQ((VariantDictionary{{"intList", std::vector<int>{5}}}),
            ToVariant(*CreateDictionaryValue("{'intList': [5]}")));
  EXPECT_EQ((VariantDictionary{
                {"intListList", std::vector<Any>{std::vector<int>{5},
                                                 std::vector<int>{6, 7}}}}),
            ToVariant(*CreateDictionaryValue(
                "{'intListList': [[5], [6, 7]]}")));
  EXPECT_EQ((VariantDictionary{{"objList",
                                std::vector<VariantDictionary>{
                                    {{"string", std::string{"abc"}}}}}}),
            ToVariant(*CreateDictionaryValue(
                "{'objList': [{'string': 'abc'}]}")));
}

TEST(DBusConversionTest, VariantDictionaryToDictionaryValue) {
  EXPECT_JSON_EQ("{'bool': true}", *FromVariant({{"bool", true}}));
  EXPECT_JSON_EQ("{'int': 5}", *FromVariant({{"int", 5}}));
  EXPECT_JSON_EQ("{'double': 6.7}", *FromVariant({{"double", 6.7}}));
  EXPECT_JSON_EQ("{'string': 'abc'}",
                 *FromVariant({{"string", std::string{"abc"}}}));
  EXPECT_JSON_EQ("{'object': {'bool': true}}",
                 *FromVariant({{"object", VariantDictionary{{"bool", true}}}}));
  EXPECT_JSON_EQ("{'emptyList': []}",
                 *FromVariant({{"emptyList", std::vector<bool>{}}}));
  EXPECT_JSON_EQ("{'intList': [5]}",
                 *FromVariant({{"intList", std::vector<int>{5}}}));
  EXPECT_JSON_EQ(
      "{'intListList': [[5], [6, 7]]}",
      *FromVariant({{"intListList",
                   std::vector<Any>{std::vector<int>{5},
                                    std::vector<int>{6, 7}}}}));
  EXPECT_JSON_EQ(
      "{'objList': [{'string': 'abc'}]}",
      *FromVariant({{"objList", std::vector<VariantDictionary>{
                                    {{"string", std::string{"abc"}}}}}}));
  EXPECT_JSON_EQ("{'int': 5}", *FromVariant({{"int", Any{Any{5}}}}));
}

TEST(DBusConversionTest, VariantDictionaryToDictionaryValueErrors) {
  EXPECT_FALSE(FromVariant({{"cString", "abc"}}));
  EXPECT_FALSE(FromVariant({{"float", 1.0f}}));
  EXPECT_FALSE(FromVariant({{"listList", std::vector<std::vector<int>>{}}}));
  EXPECT_FALSE(FromVariant({{"any", Any{}}}));
  EXPECT_FALSE(FromVariant({{"null", nullptr}}));
}

TEST(DBusConversionTest, DBusRandomDictionaryConversion) {
  auto dict = CreateRandomDictionary(10000);
  auto varian_dict = ToVariant(*dict);
  auto dict_restored = FromVariant(varian_dict);
  EXPECT_PRED2(IsEqualValue, *dict, *dict_restored);
}

}  // namespace buffet
