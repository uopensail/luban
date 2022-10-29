//
// `LuBan` - 'c++ tool for transformating and hashing features'
// Copyright (C) 2019 - present timepi <timepi123@gmail.com>
//
// This file is part of `LuBan`.
// //
// `LuBan` is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// `LuBan` is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with `LuBan`.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef LUBAN_FEATURE_OPREATOR_TOOLKIT_HPP
#define LUBAN_FEATURE_OPREATOR_TOOLKIT_HPP

#pragma once

#include "feature_builtin_operators.hpp"
#include "feature_operator_handler.hpp"
#include <unordered_map>
//定义各类函数的类型
#define SingleMapFunction                                                      \
  std::function<SharedFeaturePtr(const SharedFeaturePtr &feature,              \
                                 std::vector<RunTimeParameter> &)>
#define SingleAggFunction                                                      \
  std::function<SharedFeaturePtr(const SharedFeaturePtr &feature,              \
                                 std::vector<RunTimeParameter> &)>
#define CartesianCrossFunction                                                 \
  std::function<SharedFeaturePtr(const SharedFeaturePtr &featureA,             \
                                 const SharedFeaturePtr &featureB,             \
                                 std::vector<RunTimeParameter> &)>
#define HadamardMapFunction                                                    \
  std::function<SharedFeaturePtr(const SharedFeaturePtr &featureA,             \
                                 const SharedFeaturePtr &featureB,             \
                                 std::vector<RunTimeParameter> &)>
#define HadamardAggFunction                                                    \
  std::function<SharedFeaturePtr(const SharedFeaturePtr &featureA,             \
                                 const SharedFeaturePtr &featureB,             \
                                 std::vector<RunTimeParameter> &)>

class FeatureOperatorToolkit {
private:
  std::unordered_map<std::string, SingleMapFunction> unary_map_oprs_;
  std::unordered_map<std::string, SingleAggFunction> unary_agg_oprs_;
  std::unordered_map<std::string, CartesianCrossFunction> cartesian_cross_oprs_;
  std::unordered_map<std::string, HadamardMapFunction> hadamard_map_oprs_;
  std::unordered_map<std::string, HadamardAggFunction> hadamard_agg_oprs_;

private:
  //获得特征
  SharedFeaturePtr get(ConfigureParameter &p, RunTimeFeatures &features) {
    auto &data = p.get_data();
    auto key = to_scalar<std::string>(data);
    auto &type = p.get_type();
    switch (type) {
    case VariableType::VT_Origin_Feature:
      return get_feature_by_key(features.get_origin(), key);
    case VariableType::VT_Selected_Feature:
      return get_feature_by_key(features.get_selected(), key);
    case VariableType::VT_Anonymous_Feature:
      return get_feature_by_key(features.get_anonymous(), key);
    default:
      return nullptr;
    }
  }

  void call_realtime_func(ConfigureOperator &o, RunTimeFeatures &features) {
    const std::string &func = o.get_function();
    const std::string &name = o.get_name();
    //处理常用的内置函数
    if ("timestamp" == func) {
      SharedFeaturePtr feature = std::make_shared<tensorflow::Feature>();
      auto tmp = timestamp();
      add_value<int64_t>(feature, tmp);
      features.add_value(VariableType::VT_Anonymous_Feature, name, feature);
      return;
    } else if ("date" == func) {
      SharedFeaturePtr feature = std::make_shared<tensorflow::Feature>();
      auto tmp = date();
      add_value<std::string>(feature, tmp);
      features.add_value(VariableType::VT_Anonymous_Feature, name, feature);
      return;
    }
  }

  void call_unary_map_func(ConfigureOperator &o, RunTimeFeatures &features) {
    auto &cfg_params = o.get_parameters();
    const std::string &func = o.get_function();
    const std::string &name = o.get_name();
    VariableType type = o.get_type();
    auto iter = this->unary_map_oprs_.find(func);
    if (iter == this->unary_map_oprs_.end()) {
      return;
    }
    //获得特征
    SharedFeaturePtr feature = this->get(cfg_params->at(0), features);
    if (feature == nullptr) {
      return;
    }
    std::cout << feature->DebugString() << std::endl;

    //生成参数
    std::vector<RunTimeParameter> parameters;
    for (size_t i = 1; i < cfg_params->size(); i++) {
      parameters.push_back({cfg_params->at(i)});
    }
    auto tmp = iter->second(feature, parameters);
    if (tmp != nullptr) {
      features.add_value(type, name, tmp);
    }
  }

  void call_unary_agg_func(ConfigureOperator &o, RunTimeFeatures &features) {
    auto &cfg_params = o.get_parameters();
    const std::string &func = o.get_function();
    const std::string &name = o.get_name();
    VariableType type = o.get_type();
    auto iter = this->unary_agg_oprs_.find(func);
    if (iter == this->unary_agg_oprs_.end()) {
      return;
    }
    //获得特征
    SharedFeaturePtr feature = this->get(cfg_params->at(0), features);
    if (feature == nullptr) {
      return;
    }
    //生成参数
    std::vector<RunTimeParameter> parameters;
    for (size_t i = 1; i < cfg_params->size(); i++) {
      parameters.push_back({cfg_params->at(i)});
    }
    auto tmp = iter->second(feature, parameters);
    if (tmp != nullptr) {
      features.add_value(type, name, tmp);
    }
  }

  void call_cartesian_cross_func(ConfigureOperator &o,
                                 RunTimeFeatures &features) {
    auto &cfg_params = o.get_parameters();
    const std::string &func = o.get_function();
    const std::string &name = o.get_name();
    VariableType type = o.get_type();
    auto iter = this->cartesian_cross_oprs_.find(func);
    if (iter == this->cartesian_cross_oprs_.end()) {
      return;
    }
    //获得特征
    SharedFeaturePtr feature_1 = this->get(cfg_params->at(0), features);
    SharedFeaturePtr feature_2 = this->get(cfg_params->at(1), features);
    if (feature_1 == nullptr || feature_2 == nullptr) {
      return;
    }
    //生成参数
    std::vector<RunTimeParameter> parameters;
    for (size_t i = 2; i < cfg_params->size(); i++) {
      parameters.push_back({cfg_params->at(i)});
    }
    auto tmp = iter->second(feature_1, feature_2, parameters);
    if (tmp != nullptr) {
      features.add_value(type, name, tmp);
    }
  }

  void call_hadamard_map_func(ConfigureOperator &o, RunTimeFeatures &features) {
    auto &cfg_params = o.get_parameters();
    const std::string &func = o.get_function();
    const std::string &name = o.get_name();
    VariableType type = o.get_type();
    auto iter = this->hadamard_map_oprs_.find(func);
    if (iter == this->hadamard_map_oprs_.end()) {
      return;
    }
    //获得特征
    SharedFeaturePtr feature_1 = this->get(cfg_params->at(0), features);
    SharedFeaturePtr feature_2 = this->get(cfg_params->at(1), features);
    if (feature_1 == nullptr || feature_2 == nullptr) {
      return;
    }
    //生成参数
    std::vector<RunTimeParameter> parameters;
    for (size_t i = 2; i < cfg_params->size(); i++) {
      parameters.push_back({cfg_params->at(i)});
    }
    auto tmp = iter->second(feature_1, feature_2, parameters);
    if (tmp != nullptr) {
      features.add_value(type, name, tmp);
    }
  }

  void call_hadamard_agg_func(ConfigureOperator &o, RunTimeFeatures &features) {
    auto &cfg_params = o.get_parameters();
    const std::string &func = o.get_function();
    const std::string &name = o.get_name();
    VariableType type = o.get_type();
    auto iter = this->hadamard_agg_oprs_.find(func);
    if (iter == this->hadamard_agg_oprs_.end()) {
      return;
    }
    //获得特征
    SharedFeaturePtr feature_1 = this->get(cfg_params->at(0), features);
    SharedFeaturePtr feature_2 = this->get(cfg_params->at(1), features);
    if (feature_1 == nullptr || feature_2 == nullptr) {
      return;
    }

    //生成参数
    std::vector<RunTimeParameter> parameters;
    for (size_t i = 2; i < cfg_params->size(); i++) {
      parameters.push_back({cfg_params->at(i)});
    }
    auto tmp = iter->second(feature_1, feature_2, parameters);
    if (tmp != nullptr) {
      features.add_value(type, name, tmp);
    }
  }

public:
  FeatureOperatorToolkit() {
    add_unary_map_func_to_global_oprs(this->unary_map_oprs_, _add_0_1);
    add_unary_map_func_to_global_oprs(this->unary_map_oprs_, _add);
    add_hadamard_map_func_to_global_oprs(this->hadamard_map_oprs_, _add);
    add_cartesian_cross_func_to_global_oprs(this->cartesian_cross_oprs_, _add);

    add_unary_map_func_to_global_oprs(this->unary_map_oprs_, _sub_0_1);
    add_unary_map_func_to_global_oprs(this->unary_map_oprs_, _sub);
    add_hadamard_map_func_to_global_oprs(this->hadamard_map_oprs_, _sub);
    add_cartesian_cross_func_to_global_oprs(this->cartesian_cross_oprs_, _sub);

    add_unary_map_func_to_global_oprs(this->unary_map_oprs_, _mul_0_1);
    add_unary_map_func_to_global_oprs(this->unary_map_oprs_, _mul);
    add_hadamard_map_func_to_global_oprs(this->hadamard_map_oprs_, _mul);
    add_cartesian_cross_func_to_global_oprs(this->cartesian_cross_oprs_, _mul);

    add_unary_map_func_to_global_oprs(this->unary_map_oprs_, _div_0_1);
    add_unary_map_func_to_global_oprs(this->unary_map_oprs_, _div);
    add_hadamard_map_func_to_global_oprs(this->hadamard_map_oprs_, _div);
    add_cartesian_cross_func_to_global_oprs(this->cartesian_cross_oprs_, _div);

    add_unary_map_func_to_global_oprs(this->unary_map_oprs_, _pow_0_1);
    add_unary_map_func_to_global_oprs(this->unary_map_oprs_, _pow);
    add_hadamard_map_func_to_global_oprs(this->hadamard_map_oprs_, _pow);
    add_cartesian_cross_func_to_global_oprs(this->cartesian_cross_oprs_, _pow);

    add_unary_map_func_to_global_oprs(this->unary_map_oprs_, _mod_0_1);
    add_unary_map_func_to_global_oprs(this->unary_map_oprs_, _mod);
    add_hadamard_map_func_to_global_oprs(this->hadamard_map_oprs_, _mod);
    add_cartesian_cross_func_to_global_oprs(this->cartesian_cross_oprs_, _mod);

    add_unary_map_func_to_global_oprs(this->unary_map_oprs_, concat_0_1);
    add_unary_map_func_to_global_oprs(this->unary_map_oprs_, concat);
    add_hadamard_map_func_to_global_oprs(this->hadamard_map_oprs_, concat);
    add_cartesian_cross_func_to_global_oprs(this->cartesian_cross_oprs_,
                                            concat);

    add_unary_map_func_to_global_oprs(this->unary_map_oprs_, _floor);
    add_unary_map_func_to_global_oprs(this->unary_map_oprs_, _ceil);
    add_unary_map_func_to_global_oprs(this->unary_map_oprs_, _log);
    add_unary_map_func_to_global_oprs(this->unary_map_oprs_, _exp);

    add_unary_map_func_to_global_oprs(this->unary_map_oprs_, min_max);
    add_unary_map_func_to_global_oprs(this->unary_map_oprs_, z_score);
    add_unary_map_func_to_global_oprs(this->unary_map_oprs_, binarize);
    add_unary_map_func_to_global_oprs(this->unary_map_oprs_, box_cox);

    add_unary_agg_func_to_global_oprs(this->unary_agg_oprs_, normalize);
    add_unary_agg_func_to_global_oprs(this->unary_agg_oprs_, topk);
  }

  ~FeatureOperatorToolkit() {}
  void call(ConfigureOperator &o, RunTimeFeatures &features) {
    switch (o.get_function_type()) {
    case FunctionType::FT_Unary_Mapper_Func:
      this->call_unary_map_func(o, features);
      return;
    case FunctionType::FT_Unary_Aggregate_Func:
      this->call_unary_agg_func(o, features);
      return;
    case FunctionType::FT_Cartesian_Cross_Func:
      this->call_cartesian_cross_func(o, features);
      return;
    case FunctionType::FT_Hadamard_Mapper_Func:
      this->call_hadamard_map_func(o, features);
      return;
    case FunctionType::FT_Hadamard_Aggregate_Func:
      this->call_hadamard_agg_func(o, features);
      return;
    case FunctionType::FT_RealTime_Func:
      this->call_realtime_func(o, features);
      return;
    default:
      return;
    }
  }
};

#endif // LUBAN_FEATURE_OPREATOR_TOOLKIT_HPP