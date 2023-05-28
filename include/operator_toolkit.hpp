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

#ifndef LUBAN_FEATURE_OPREATOR_TOOLKIT_HPP
#define LUBAN_FEATURE_OPREATOR_TOOLKIT_HPP

#pragma once

#include <unordered_map>

#include "binary_handler.hpp"
#include "builtin_operators.hpp"
#include "configure.hpp"
#include "simple_handler.hpp"
#include "unary_handler.hpp"

class FeatureOperatorToolkit {
private:
  void call_simple_func(const ConfigureOperator &o, RunTimeFeatures &features) {
    const std::string &func = o.get_function();
    const std::string &name = o.get_name();
    auto iter = this->simple_callers_.find(func);
    if (iter == this->simple_callers_.end()) {
      return;
    }
    if (iter->second == nullptr) {
      return;
    }
    auto tmp = do_simple_call(*iter->second);
    if (tmp != nullptr) {
      features.insert(name, tmp);
      // std::cout << tmp->DebugString() << std::endl;
    }
  }

  void call_unary_func(const ConfigureOperator &o, RunTimeFeatures &features) {
    const std::string &func = o.get_function();
    const std::string &name = o.get_name();
    auto iter = this->unary_callers_.find(func);
    if (iter == this->unary_callers_.end()) {
      return;
    }
    if (iter->second == nullptr) {
      return;
    }
    auto tmp = do_unary_call(*iter->second, features);
    if (tmp != nullptr) {
      features.insert(name, tmp);
      // std::cout << tmp->DebugString() << std::endl;
    }
  }

  void call_binary_func(const ConfigureOperator &o, RunTimeFeatures &features) {
    const std::string &func = o.get_function();
    const std::string &name = o.get_name();
    auto iter = this->binary_callers_.find(func);
    if (iter == this->binary_callers_.end()) {
      return;
    }

    if (iter->second == nullptr) {
      return;
    }
    auto tmp = do_binary_call(*iter->second, features);

    if (tmp != nullptr) {
      features.insert(name, tmp);
      // std::cout << tmp->DebugString() << std::endl;
    }
  }

public:
  FeatureOperatorToolkit(const std::vector<ConfigureOperator> &transforms) {
    std::unordered_map<std::string, size_t> unary_map;
    std::unordered_map<std::string, size_t> binray_map;
    for (size_t i = 0; i < transforms.size(); i++) {
      transforms_.push_back(transforms[i]);
      if (transforms[i].get_function_type() == FunctionType::FT_Unary_Func) {
        unary_map[transforms[i].get_function()] = i;
      } else if (transforms[i].get_function_type() ==
                 FunctionType::FT_Binary_Func) {
        binray_map[transforms[i].get_function()] = i;
      }
    }

// add simple caller
#define add_simple_caller(func)                                                \
  { simple_callers_[#func] = get_simple_caller(func); }

    add_simple_caller(now);
    add_simple_caller(year);
    add_simple_caller(month);
    add_simple_caller(day);
    add_simple_caller(hour);
    add_simple_caller(minute);
    add_simple_caller(second);
    add_simple_caller(date);

// add unary caller
#define add_unary_caller(func)                                                 \
  {                                                                            \
    auto it = unary_map.find(#func);                                           \
    if (it != unary_map.end()) {                                               \
      unary_callers_[#func] = get_unary_caller(func, transforms[it->second]);  \
    }                                                                          \
  }
    add_unary_caller(_add);
    add_unary_caller(_sub);
    add_unary_caller(_mul);
    add_unary_caller(_div);
    add_unary_caller(_mod);
    add_unary_caller(_pow);
    add_unary_caller(_round);
    add_unary_caller(_floor);
    add_unary_caller(_ceil);
    add_unary_caller(_log);
    add_unary_caller(_exp);
    add_unary_caller(_log10);
    add_unary_caller(_log2);
    add_unary_caller(_sqrt);
    add_unary_caller(_abs);
    add_unary_caller(_sin);
    add_unary_caller(_sinh);
    add_unary_caller(_asin);
    add_unary_caller(_asinh);
    add_unary_caller(_cos);
    add_unary_caller(_cosh);
    add_unary_caller(_acos);
    add_unary_caller(_acosh);
    add_unary_caller(_tan);
    add_unary_caller(_tanh);
    add_unary_caller(_atan);
    add_unary_caller(_atanh);
    add_unary_caller(_sigmoid);
    add_unary_caller(min);
    add_unary_caller(max);
    add_unary_caller(stddev);
    add_unary_caller(variance);
    add_unary_caller(norm);
    add_unary_caller(from_unixtime);
    add_unary_caller(unix_timestamp);
    add_unary_caller(date_add);
    add_unary_caller(date_sub);
    add_unary_caller(date_diff);
    add_unary_caller(reverse);
    add_unary_caller(upper);
    add_unary_caller(lower);
    add_unary_caller(substr);
    add_unary_caller(concat);
    add_unary_caller(min_max);
    add_unary_caller(z_score);
    add_unary_caller(binarize);
    add_unary_caller(bucketize);
    add_unary_caller(box_cox);
    add_unary_caller(normalize);
    add_unary_caller(topks);
    add_unary_caller(topki);
    add_unary_caller(topkf);

// add binary caller
#define add_binary_caller(func)                                                \
  {                                                                            \
    auto it = binray_map.find(#func);                                          \
    if (it != binray_map.end()) {                                              \
      binary_callers_[#func] =                                                 \
          get_binary_caller(func, transforms[it->second]);                     \
    }                                                                          \
  }
    add_binary_caller(_add);
    add_binary_caller(_sub);
    add_binary_caller(_mul);
    add_binary_caller(_div);
    add_binary_caller(_mod);
    add_binary_caller(_pow);
    add_binary_caller(date_add);
    add_binary_caller(date_sub);
    add_binary_caller(date_diff);
    add_binary_caller(concat);
    add_binary_caller(cross);
  }

  ~FeatureOperatorToolkit() {}
  void call(RunTimeFeatures &features) {
    for (const auto &transform : transforms_) {
      switch (transform.get_function_type()) {
      case FunctionType::FT_Unary_Func:
        this->call_unary_func(transform, features);
        break;
      case FunctionType::FT_Binary_Func:
        this->call_binary_func(transform, features);
        break;
      case FunctionType::FT_Simple_Func:
        this->call_simple_func(transform, features);
        break;
      default:
        break;
      }
    }
  }

private:
  std::unordered_map<std::string, std::shared_ptr<SimpleCaller>>
      simple_callers_;
  std::unordered_map<std::string, std::shared_ptr<UnaryCaller>> unary_callers_;
  std::unordered_map<std::string, std::shared_ptr<BinaryCaller>>
      binary_callers_;
  std::vector<ConfigureOperator> transforms_;
};

#endif // LUBAN_FEATURE_OPREATOR_TOOLKIT_HPP