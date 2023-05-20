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
#include "unary_handler.hpp"

class FeatureOperatorToolkit {
private:
  void call_simple_func(const ConfigureOperator &o, RunTimeFeatures &features) {
    const std::string &func = o.get_function();
    const std::string &name = o.get_name();
    const VariableType &type = o.get_type();
    const auto &cfg_params = o.get_parameters();
    // process simple functions
    if ("now" == func) {
      SharedFeaturePtr feature = std::make_shared<sample::Feature>();
      auto tmp = now();
      add_value<int64_t>(feature, tmp);
      features.insert(type, name, feature);
      return;
    } else if ("year" == func) {
      SharedFeaturePtr feature = std::make_shared<sample::Feature>();
      auto tmp = year();
      add_value<std::string>(feature, tmp);
      features.insert(type, name, feature);
      return;
    } else if ("month" == func) {
      SharedFeaturePtr feature = std::make_shared<sample::Feature>();
      auto tmp = month();
      add_value<std::string>(feature, tmp);
      features.insert(type, name, feature);
      return;
    } else if ("day" == func) {
      SharedFeaturePtr feature = std::make_shared<sample::Feature>();
      auto tmp = day();
      add_value<std::string>(feature, tmp);
      features.insert(type, name, feature);
      return;
    } else if ("hour" == func) {
      SharedFeaturePtr feature = std::make_shared<sample::Feature>();
      auto tmp = hour();
      add_value<std::string>(feature, tmp);
      features.insert(type, name, feature);
      return;
    } else if ("minute" == func) {
      SharedFeaturePtr feature = std::make_shared<sample::Feature>();
      auto tmp = minute();
      add_value<std::string>(feature, tmp);
      features.insert(type, name, feature);
      return;
    } else if ("second" == func) {
      SharedFeaturePtr feature = std::make_shared<sample::Feature>();
      auto tmp = second();
      add_value<std::string>(feature, tmp);
      features.insert(type, name, feature);
      return;
    } else if ("_identity" == func) {
      SharedFeaturePtr feature = features.get(cfg_params->at(0));
      if (feature == nullptr) {
        return;
      }
      features.insert(type, name, feature);
      return;
    }
  }

  void call_unary_func(const ConfigureOperator &o, RunTimeFeatures &features) {
    const std::string &func = o.get_function();
    const std::string &name = o.get_name();
    const VariableType &type = o.get_type();
    auto iter = this->unary_callers_.find(func);
    if (iter == this->unary_callers_.end()) {
      return;
    }

    auto tmp = do_unary_call(*iter->second, features);
    if (tmp != nullptr) {
      features.insert(type, name, tmp);
      // std::cout << tmp->DebugString() << std::endl;
    }
  }

  void call_binary_func(const ConfigureOperator &o, RunTimeFeatures &features) {
    const std::string &func = o.get_function();
    const std::string &name = o.get_name();
    const VariableType &type = o.get_type();
    auto iter = this->binary_callers_.find(func);
    if (iter == this->binary_callers_.end()) {
      return;
    }

    auto tmp = do_binary_call(*iter->second, features);

    if (tmp != nullptr) {
      features.insert(type, name, tmp);
    }
  }

public:
  FeatureOperatorToolkit(const std::vector<ConfigureOperator> &oprs) {
    std::unordered_map<std::string, size_t> unary_oprs_map;
    std::unordered_map<std::string, size_t> binray_oprs_map;
    for (size_t i = 0; i < oprs.size(); i++) {
      if (oprs[i].get_function_type() == FunctionType::FT_Unary_Func) {
        unary_oprs_map[oprs[i].get_function()] = i;
      } else if (oprs[i].get_function_type() == FunctionType::FT_Binary_Func) {
        binray_oprs_map[oprs[i].get_function()] = i;
      }
    }

// add unary caller
#define add_unary_caller(caller_map, func, dict)                               \
  {                                                                            \
    auto it = dict.find(#func);                                                \
    if (it != dict.end()) {                                                    \
      caller_map[#func] = get_unary_caller(func, oprs[it->second]);            \
    }                                                                          \
  }
    add_unary_caller(unary_callers_, _minf, unary_oprs_map);
    add_unary_caller(unary_callers_, _mins, unary_oprs_map);
    add_unary_caller(unary_callers_, _mini, unary_oprs_map);
    add_unary_caller(unary_callers_, _maxf, unary_oprs_map);
    add_unary_caller(unary_callers_, _maxs, unary_oprs_map);
    add_unary_caller(unary_callers_, _maxi, unary_oprs_map);
    add_unary_caller(unary_callers_, _addf, unary_oprs_map);
    add_unary_caller(unary_callers_, _addi, unary_oprs_map);
    add_unary_caller(unary_callers_, _subf, unary_oprs_map);
    add_unary_caller(unary_callers_, _subi, unary_oprs_map);
    add_unary_caller(unary_callers_, _mulf, unary_oprs_map);
    add_unary_caller(unary_callers_, _muli, unary_oprs_map);
    add_unary_caller(unary_callers_, _divf, unary_oprs_map);
    add_unary_caller(unary_callers_, _divi, unary_oprs_map);
    add_unary_caller(unary_callers_, _modi, unary_oprs_map);
    add_unary_caller(unary_callers_, _powi, unary_oprs_map);
    add_unary_caller(unary_callers_, _powf, unary_oprs_map);
    add_unary_caller(unary_callers_, _round, unary_oprs_map);
    add_unary_caller(unary_callers_, _floor, unary_oprs_map);
    add_unary_caller(unary_callers_, _ceil, unary_oprs_map);
    add_unary_caller(unary_callers_, _log, unary_oprs_map);
    add_unary_caller(unary_callers_, _exp, unary_oprs_map);
    add_unary_caller(unary_callers_, _log10, unary_oprs_map);
    add_unary_caller(unary_callers_, _log2, unary_oprs_map);
    add_unary_caller(unary_callers_, _sqrt, unary_oprs_map);
    add_unary_caller(unary_callers_, _absf, unary_oprs_map);
    add_unary_caller(unary_callers_, _absi, unary_oprs_map);
    add_unary_caller(unary_callers_, _sin, unary_oprs_map);
    add_unary_caller(unary_callers_, _sinh, unary_oprs_map);
    add_unary_caller(unary_callers_, _asin, unary_oprs_map);
    add_unary_caller(unary_callers_, _asinh, unary_oprs_map);
    add_unary_caller(unary_callers_, _cos, unary_oprs_map);
    add_unary_caller(unary_callers_, _cosh, unary_oprs_map);
    add_unary_caller(unary_callers_, _acos, unary_oprs_map);
    add_unary_caller(unary_callers_, _acosh, unary_oprs_map);
    add_unary_caller(unary_callers_, _tan, unary_oprs_map);
    add_unary_caller(unary_callers_, _tanh, unary_oprs_map);
    add_unary_caller(unary_callers_, _atan, unary_oprs_map);
    add_unary_caller(unary_callers_, _atanh, unary_oprs_map);
    add_unary_caller(unary_callers_, from_unixtime, unary_oprs_map);
    add_unary_caller(unary_callers_, unix_timestamp, unary_oprs_map);
    add_unary_caller(unary_callers_, date_add, unary_oprs_map);
    add_unary_caller(unary_callers_, date_sub, unary_oprs_map);
    add_unary_caller(unary_callers_, date_diff, unary_oprs_map);
    add_unary_caller(unary_callers_, reverse, unary_oprs_map);
    add_unary_caller(unary_callers_, upper, unary_oprs_map);
    add_unary_caller(unary_callers_, lower, unary_oprs_map);
    add_unary_caller(unary_callers_, substr, unary_oprs_map);
    add_unary_caller(unary_callers_, concat, unary_oprs_map);
    add_unary_caller(unary_callers_, _to_stringf, unary_oprs_map);
    add_unary_caller(unary_callers_, _to_stringi, unary_oprs_map);
    add_unary_caller(unary_callers_, _to_integerf, unary_oprs_map);
    add_unary_caller(unary_callers_, _to_integers, unary_oprs_map);
    add_unary_caller(unary_callers_, _to_floati, unary_oprs_map);
    add_unary_caller(unary_callers_, _to_floats, unary_oprs_map);
    add_unary_caller(unary_callers_, min_max, unary_oprs_map);
    add_unary_caller(unary_callers_, z_score, unary_oprs_map);
    add_unary_caller(unary_callers_, binarize, unary_oprs_map);
    add_unary_caller(unary_callers_, bucketize, unary_oprs_map);
    add_unary_caller(unary_callers_, box_cox, unary_oprs_map);
    add_unary_caller(unary_callers_, normalize, unary_oprs_map);
    add_unary_caller(unary_callers_, topks, unary_oprs_map);
    add_unary_caller(unary_callers_, topki, unary_oprs_map);
    add_unary_caller(unary_callers_, topkf, unary_oprs_map);

// add binary caller
#define add_binary_caller(caller_map, func, dict)                              \
  {                                                                            \
    auto it = dict.find(#func);                                                \
    if (it != dict.end()) {                                                    \
      caller_map[#func] = get_binary_caller(func, oprs[it->second]);           \
    }                                                                          \
  }
    add_binary_caller(binary_callers_, _mini, binray_oprs_map);
    add_binary_caller(binary_callers_, _mins, binray_oprs_map);
    add_binary_caller(binary_callers_, _minf, binray_oprs_map);
    add_binary_caller(binary_callers_, _maxi, binray_oprs_map);
    add_binary_caller(binary_callers_, _maxs, binray_oprs_map);
    add_binary_caller(binary_callers_, _maxf, binray_oprs_map);
    add_binary_caller(binary_callers_, _addi, binray_oprs_map);
    add_binary_caller(binary_callers_, _addf, binray_oprs_map);
    add_binary_caller(binary_callers_, _subi, binray_oprs_map);
    add_binary_caller(binary_callers_, _subf, binray_oprs_map);
    add_binary_caller(binary_callers_, _muli, binray_oprs_map);
    add_binary_caller(binary_callers_, _mulf, binray_oprs_map);
    add_binary_caller(binary_callers_, _divi, binray_oprs_map);
    add_binary_caller(binary_callers_, _divf, binray_oprs_map);
    add_binary_caller(binary_callers_, _modi, binray_oprs_map);
    add_binary_caller(binary_callers_, _powi, binray_oprs_map);
    add_binary_caller(binary_callers_, _powf, binray_oprs_map);
    add_binary_caller(binary_callers_, date_add, binray_oprs_map);
    add_binary_caller(binary_callers_, date_sub, binray_oprs_map);
    add_binary_caller(binary_callers_, date_diff, binray_oprs_map);
    add_binary_caller(binary_callers_, concat, binray_oprs_map);
  }

  ~FeatureOperatorToolkit() {}
  void call(const ConfigureOperator &o, RunTimeFeatures &features) {
    switch (o.get_function_type()) {
    case FunctionType::FT_Unary_Func:
      this->call_unary_func(o, features);
      return;
    case FunctionType::FT_Binary_Func:
      this->call_binary_func(o, features);
      return;
    case FunctionType::FT_Simple_Func:
      this->call_simple_func(o, features);
      return;
    default:
      return;
    }
  }

private:
  std::unordered_map<std::string, std::shared_ptr<UnaryCaller>> unary_callers_;
  std::unordered_map<std::string, std::shared_ptr<BinaryCaller>>
      binary_callers_;
};

#endif // LUBAN_FEATURE_OPREATOR_TOOLKIT_HPP