/*
 * Open Chinese Convert
 *
 * Copyright 2010-2014 Carbo Kuo <byvoid@byvoid.com>
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

#ifdef _MSC_VER
#define NOMINMAX
#include <Windows.h>
#undef NOMINMAX
#endif // _MSC_VER

#include "SimpleConverter.hpp"
#include "Config.hpp"
#include "Converter.hpp"
#include "UTF8Util.hpp"
#include "opencc.h"

using namespace opencc;

struct InternalData {
  const ConverterPtr converter;

  InternalData(const ConverterPtr& _converter) : converter(_converter) {}
};

SimpleConverter::SimpleConverter(const std::string& configFileName) {
  try {
    Config config;
    internalData = new InternalData(config.NewFromFile(configFileName));
  } catch (Exception& ex) {
    throw std::runtime_error(ex.what());
  }
}

SimpleConverter::~SimpleConverter() { delete (InternalData*)internalData; }

std::string SimpleConverter::Convert(const std::string& input) const {
  try {
    const InternalData* data = (InternalData*)internalData;
    return data->converter->Convert(input);
  } catch (Exception& ex) {
    throw std::runtime_error(ex.what());
  }
}

std::string SimpleConverter::Convert(const char* input) const {
  return Convert(std::string(input));
}

std::string SimpleConverter::Convert(const char* input, size_t length) const {
  if (length == static_cast<size_t>(-1)) {
    return Convert(std::string(input));
  } else {
    return Convert(UTF8Util::FromSubstr(input, length));
  }
}

size_t SimpleConverter::Convert(const char* input, char* output) const {
  try {
    const InternalData* data = (InternalData*)internalData;
    return data->converter->Convert(input, output);
  } catch (Exception& ex) {
    throw std::runtime_error(ex.what());
  }
}

size_t SimpleConverter::Convert(const char* input, size_t length,
                                char* output) const {
  if (length == static_cast<size_t>(-1)) {
    return Convert(input, output);
  } else {
    std::string trimmed = UTF8Util::FromSubstr(input, length);
    return Convert(trimmed.c_str(), output);
  }
}