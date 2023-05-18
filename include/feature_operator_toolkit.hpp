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

#include "feature_builtin_operators.hpp"
#include "feature_operator_handler.hpp"

#define UnaryFunction \
  std::function<SharedFeaturePtr(RunTimeFeatures &, const ConfigureOperator &)>

#define BinaryFunction \
  std::function<SharedFeaturePtr(RunTimeFeatures &, const ConfigureOperator &)>

class FeatureOperatorToolkit {
 private:
  std::unordered_map<std::string, UnaryFunction> unary_oprs_;
  std::unordered_map<std::string, BinaryFunction> binary_oprs_;

 private:
  void call_simple_func(const ConfigureOperator &o, RunTimeFeatures &features) {
    const std::string &func = o.get_function();
    const std::string &name = o.get_name();
    const VariableType &type = o.get_type();
    const auto &cfg_params = o.get_parameters();
    // process built in functions
    if ("timestamp" == func) {
      SharedFeaturePtr feature = std::make_shared<sample::Feature>();
      auto tmp = timestamp();
      add_value<int64_t>(feature, tmp);
      features.insert(type, name, feature);
      return;
    } else if ("date" == func) {
      SharedFeaturePtr feature = std::make_shared<sample::Feature>();
      auto tmp = date();
      add_value<std::string>(feature, tmp);
      features.insert(type, name, feature);
      return;
    } else if ("_identity" == func) {
      //获得特征
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
    auto iter = this->unary_oprs_.find(func);
    if (iter == this->unary_oprs_.end()) {
      return;
    }

    auto tmp = iter->second(features, o);

    if (tmp != nullptr) {
      features.insert(type, name, tmp);
      // std::cout << tmp->DebugString() << std::endl;
    }
  }

  void call_binary_func(const ConfigureOperator &o, RunTimeFeatures &features) {
    const std::string &func = o.get_function();
    const std::string &name = o.get_name();
    const VariableType &type = o.get_type();
    auto iter = this->binary_oprs_.find(func);
    if (iter == this->binary_oprs_.end()) {
      return;
    }

    auto tmp = iter->second(features, o);

    if (tmp != nullptr) {
      features.insert(type, name, tmp);
    }
  }

 public:
  FeatureOperatorToolkit() {
    add_unary_func_to_global_oprs(this->unary_oprs_, _add);
    add_binary_func_to_global_oprs(this->binary_oprs_, _add);

    add_unary_func_to_global_oprs(this->unary_oprs_, _sub);
    add_binary_func_to_global_oprs(this->binary_oprs_, _sub);

    add_unary_func_to_global_oprs(this->unary_oprs_, _mul);
    add_binary_func_to_global_oprs(this->binary_oprs_, _mul);

    add_unary_func_to_global_oprs(this->unary_oprs_, _div);
    add_binary_func_to_global_oprs(this->binary_oprs_, _div);

    add_unary_func_to_global_oprs(this->unary_oprs_, _pow);
    add_binary_func_to_global_oprs(this->binary_oprs_, _pow);

    add_unary_func_to_global_oprs(this->unary_oprs_, _mod);
    add_binary_func_to_global_oprs(this->binary_oprs_, _mod);

    add_unary_func_to_global_oprs(this->unary_oprs_, concat);
    add_binary_func_to_global_oprs(this->binary_oprs_, concat);

    add_unary_func_to_global_oprs(this->unary_oprs_, _floor);
    add_unary_func_to_global_oprs(this->unary_oprs_, _ceil);
    add_unary_func_to_global_oprs(this->unary_oprs_, _log);
    add_unary_func_to_global_oprs(this->unary_oprs_, _exp);
    add_unary_func_to_global_oprs(this->unary_oprs_, substr);

    add_unary_func_to_global_oprs(this->unary_oprs_, min_max);
    add_unary_func_to_global_oprs(this->unary_oprs_, z_score);
    add_unary_func_to_global_oprs(this->unary_oprs_, binarize);
    add_unary_func_to_global_oprs(this->unary_oprs_, bucketize);
    add_unary_func_to_global_oprs(this->unary_oprs_, box_cox);

    add_unary_func_to_global_oprs(this->unary_oprs_, normalize);
    add_unary_func_to_global_oprs(this->unary_oprs_, topki);
    add_unary_func_to_global_oprs(this->unary_oprs_, topkf);
    add_unary_func_to_global_oprs(this->unary_oprs_, topks);
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
};

#endif  // LUBAN_FEATURE_OPREATOR_TOOLKIT_HPP