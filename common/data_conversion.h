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

#ifndef COMMON_DATA_CONVERSION_H_
#define COMMON_DATA_CONVERSION_H_

#include <base/values.h>
#include <brillo/any.h>
#include <brillo/errors/error.h>
#include <brillo/variant_dictionary.h>

namespace weaved {
namespace details {

// Converts DictionaryValue to variant dictionary.
brillo::VariantDictionary DictionaryValueToVariantDictionary(
    const base::DictionaryValue& object);

// Converts variant dictionary to DictionaryValue.
std::unique_ptr<base::DictionaryValue> VariantDictionaryToDictionaryValue(
    const brillo::VariantDictionary& object,
    brillo::ErrorPtr* error);

}  // namespace details
}  // namespace weaved

#endif  // COMMON_DATA_CONVERSION_H_
