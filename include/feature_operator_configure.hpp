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

#ifndef LUBAN_FEATURE_OPREATOR_CONFIGURE_HPP
#define LUBAN_FEATURE_OPREATOR_CONFIGURE_HPP

#pragma once

//特征处理算子的配置处理

#include "cpptoml.h"
#include "feature_helper.hpp"
#include "helper.h"
#include <iostream>
#include <string>

/*
    VT_Int: 字面整型常量
    VT_Float: 字面浮点型常量
    VT_String: 字面字符串常量, 以引号作为字符串的开始和结束
    VT_IntList: 字面整型常量数组
    VT_FloatList: 字面浮点型常量数组
    VT_StringList: 字面字符串常量数组
    VT_Original_Feature: 原始特征, tffeature类型的值, TensorFlow中定义
    VT_Selected_Feature: 命名的变量, tffeature类型的值
    VT_Anonymous_Feature: 匿名变量, tffeature类型的值
*/
enum VariableType {
  VT_Not_Defined = 0,
  VT_Int,
  VT_Float,
  VT_String,
  VT_IntList,
  VT_FloatList,
  VT_StringList,
  VT_Origin_Feature,
  VT_Selected_Feature,
  VT_Anonymous_Feature
};

//函数的类型
enum FunctionType {
  FT_Not_Defined = 0,
  FT_RealTime_Func,
  FT_Unary_Mapper_Func,
  FT_Unary_Aggregate_Func,
  FT_Hadamard_Mapper_Func,
  FT_Hadamard_Aggregate_Func
};

//配置中的参数
class ConfigureParameter {
private:
  int index_;
  VariableType type_;
  SharedFeaturePtr data_;

public:
  ConfigureParameter() = delete;
  ConfigureParameter(int index, VariableType type, const SharedFeaturePtr &data)
      : index_(index), type_(type), data_(data) {}
  ConfigureParameter(const ConfigureParameter &p)
      : index_(p.index_), type_(p.type_), data_(p.data_) {}

  //从toml配置中生成ConfigureParameter
  ConfigureParameter(const std::shared_ptr<cpptoml::table> &table) {
    //配置前置条件检查
    assert(table->contains("index") && table->contains("type") &&
           table->contains("data"));
    ParamsHelper params(table);
    this->index_ = params.get<int>("index");
    this->type_ = static_cast<VariableType>(params.get<int>("type"));
    this->data_ = SharedFeaturePtr(new tensorflow::Feature());

    switch (this->type_) {
    case VariableType::VT_Int:
      this->data_->mutable_int64_list()->add_value(params.get<int64_t>("data"));
      break;
    case VariableType::VT_IntList:
      for (const auto &v : params.get_array<int64_t>("data")) {
        this->data_->mutable_int64_list()->add_value(v);
      }
      break;
    case VariableType::VT_Float:
      this->data_->mutable_float_list()->add_value(
          static_cast<float>(params.get<double>("data")));
      break;
    case VariableType::VT_FloatList:
      for (const auto &v : params.get_array<double>("data")) {
        this->data_->mutable_float_list()->add_value(static_cast<float>(v));
      }
      break;
    case VariableType::VT_StringList:
      for (const auto &v : params.get_array<std::string>("data")) {
        this->data_->mutable_bytes_list()->add_value(v);
      }
      break;

    case VariableType::VT_String:
    case VariableType::VT_Origin_Feature:
    case VariableType::VT_Selected_Feature:
    case VariableType::VT_Anonymous_Feature:
      this->data_->mutable_bytes_list()->add_value(
          params.get<std::string>("data"));
      break;
    default:
      break;
    }
  }

  ConfigureParameter &operator=(const ConfigureParameter &p) {
    if (this == &p) {
      return *this;
    }
    this->index_ = p.index_;
    this->type_ = p.type_;
    this->data_ = p.data_;
    return *this;
  }
  ~ConfigureParameter() {}
  const int &get_index() { return this->index_; }
  const VariableType &get_type() { return this->type_; }
  const SharedFeaturePtr &get_data() { return this->data_; }
};

//配置中的operator
class ConfigureOperator {
private:
  VariableType type_;
  FunctionType func_type_;
  std::string name_;
  std::string function_;
  std::shared_ptr<std::vector<ConfigureParameter>> parameters_;

public:
  ConfigureOperator() = delete;
  ConfigureOperator(const ConfigureOperator &o)
      : type_(o.type_), func_type_(o.func_type_), name_(o.name_),
        function_(o.function_), parameters_(o.parameters_) {}
  ConfigureOperator &operator=(const ConfigureOperator &o) {
    if (this == &o) {
      return *this;
    }
    this->name_ = o.name_;
    this->function_ = o.function_;
    this->parameters_ = o.parameters_;
    this->type_ = o.type_;
    this->func_type_ = o.func_type_;
    return *this;
  }

  ConfigureOperator(const std::shared_ptr<cpptoml::table> &table) {
    assert(table->contains("name") && table->contains("func") &&
           table->contains("type") && table->contains("func_type"));
    ParamsHelper params(table);
    this->name_ = params.get<std::string>("name");
    this->function_ = params.get<std::string>("func");
    this->type_ = static_cast<VariableType>(params.get<int>("type"));
    this->func_type_ = static_cast<FunctionType>(params.get<int>("func_type"));
    this->parameters_ = std::shared_ptr<std::vector<ConfigureParameter>>(
        new std::vector<ConfigureParameter>());

    if (table->contains("params")) {
      for (const auto &t : *table->get_table_array("params")) {
        this->parameters_->push_back(ConfigureParameter{t});
      }
    }
  }
  ~ConfigureOperator() {}
  const std::string &get_name() { return this->name_; }
  const std::string &get_function() { return this->function_; }
  const VariableType &get_type() { return this->type_; }
  const FunctionType &get_function_type() { return this->func_type_; }
  const std::shared_ptr<std::vector<ConfigureParameter>> &get_parameters() {
    return this->parameters_;
  }
};

#endif // LUBAN_FEATURE_OPREATOR_CONFIGURE_HPP