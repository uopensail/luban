//
// `LuBan` - 'c++ tool for transformating and hashing features'
// Copyright (C) 2019 - present timepi <timepi123@gmail.com>
// LuBan is provided under: GNU Affero General Public License (AGPL3.0)
// https://www.gnu.org/licenses/agpl-3.0.html unless stated otherwise.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//

#ifndef LUBAN_COMMON_H
#define LUBAN_COMMON_H

#pragma once

#include <assert.h>

#include "MurmurHash3.h"
#include "builtin.h"
#include "helper.h"
#include "json.hpp"

#include <algorithm>
#include <functional>
#include <map>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

using json = nlohmann::json;

namespace luban {

using Parameter =
    std::variant<int64_t, float, std::string, std::vector<int64_t>,
                 std::vector<float>, std::vector<std::string>>;

using Padding = std::variant<int64_t, float>;

using SharedParameter = std::shared_ptr<Parameter>;

using SharedParameterMap = std::unordered_map<std::string, SharedParameter>;

enum DataType : int32_t {
  kInt64 = 0,
  kFloat32,
  kString,
  kInt64Array,
  kFloat32Array,
  kStringArray,
};

typedef struct FunctionConfigure {
  std::string func;
  std::string name;
  int64_t flag;
  std::vector<std::string> variables;
  std::vector<SharedParameter> literals;
  FunctionConfigure(const std::string &data);
  FunctionConfigure(const json &data);
  void parse(const json &data);
  std::string stringnify();
} FunctionConfigure;

template <typename T>
static std::vector<T> to_vector(const SharedParameter &data) {
  throw std::runtime_error("type not support: " + get_template_type<T>());
}

template <> std::vector<std::string> to_vector(const SharedParameter &data) {
  std::vector<std::string> ret;
  if (data == nullptr) {
    return ret;
  }
  struct _wrapper {
    void operator()(int64_t arg) { values.emplace_back(std::to_string(arg)); }
    void operator()(float arg) { values.emplace_back(std::to_string(arg)); }
    void operator()(const std::string &arg) { values.emplace_back(arg); }
    void operator()(const std::vector<int64_t> &arg) {
      for (size_t i = 0; i < arg.size(); ++i) {
        values.push_back(std::to_string(arg[i]));
      }
    }
    void operator()(const std::vector<float> &arg) {
      for (size_t i = 0; i < arg.size(); ++i) {
        values.push_back(std::to_string(arg[i]));
      }
    }
    void operator()(const std::vector<std::string> &arg) {
      for (size_t i = 0; i < arg.size(); ++i) {
        values.push_back(arg[i]);
      }
    }
    std::vector<std::string> &values;
  };
  _wrapper w{ret};
  std::visit(w, *data);
  return ret;
}

template <> std::vector<int64_t> to_vector(const SharedParameter &data) {
  std::vector<int64_t> ret;
  if (data == nullptr) {
    return ret;
  }
  struct _wrapper {
    void operator()(int64_t arg) { values.emplace_back(arg); }
    void operator()(float arg) {
      values.emplace_back(static_cast<int64_t>(arg));
    }
    void operator()(const std::vector<int64_t> &arg) {
      for (size_t i = 0; i < arg.size(); ++i) {
        values.push_back(arg[i]);
      }
    }
    void operator()(const std::vector<float> &arg) {
      for (size_t i = 0; i < arg.size(); ++i) {
        values.push_back(static_cast<int64_t>(arg[i]));
      }
    }
    void operator()(const std::vector<std::string> &arg) {}
    void operator()(const std::string &arg) {}
    std::vector<int64_t> &values;
  };
  _wrapper w{ret};

  std::visit(w, *data);
  return ret;
}

template <> std::vector<float> to_vector(const SharedParameter &data) {
  std::vector<float> ret;
  if (data == nullptr) {
    return ret;
  }
  struct _wrapper {
    void operator()(float arg) { values.emplace_back(arg); }
    void operator()(int64_t arg) {
      values.emplace_back(static_cast<float>(arg));
    }
    void operator()(const std::vector<float> &arg) {
      for (size_t i = 0; i < arg.size(); ++i) {
        values.push_back(arg[i]);
      }
    }
    void operator()(const std::vector<int64_t> &arg) {
      for (size_t i = 0; i < arg.size(); ++i) {
        values.push_back(static_cast<float>(arg[i]));
      }
    }
    void operator()(const std::vector<std::string> &arg) {}
    void operator()(const std::string &arg) {}
    std::vector<float> &values;
  };
  _wrapper w{ret};

  std::visit(w, *data);
  return ret;
}

template <typename T>
static std::vector<T> placement(const SharedParameter &feature, bool hash,
                                Padding padding, int64_t size) {
  if constexpr (!(std::is_same_v<T, int64_t> || std::is_same_v<T, float>)) {
    throw std::runtime_error("type not support: " + get_template_type<T>());
  }

  if (hash) {
    if constexpr (!std::is_same_v<T, int64_t>) {
      throw std::runtime_error("hash result must be int64_t type");
    }

    auto array = to_vector<std::string>(feature);
    std::vector<T> ret(size, std::get<T>(padding));
    int64_t length = std::min<int64_t>(array.size(), size);
    for (int64_t i = 0; i < length; ++i) {
      ret[i] = mmh3(array[i]);
    }
    return ret;
  }

  auto array = to_vector<T>(feature);
  array.resize(size, std::get<T>(padding));
  return array;
}

SharedParameter parse_parameter_from_json(const json &data);
SharedParameter parse_parameter_from_string(const std::string &data);

void print_parameter(const SharedParameter &param);

std::string parameter_stringnify(const SharedParameter &param);

void extract_features(std::string_view data, SharedParameterMap &features);

} // namespace luban
#endif // LUBAN_COMMON_H