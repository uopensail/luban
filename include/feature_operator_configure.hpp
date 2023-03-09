//
// `LuBan` - 'c++ tool for transformating and hashing features'
// Copyright (C) 2019 - present timepi <timepi123@gmail.com>
//
// This file is part of `LuBan`.
//
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

#include <iostream>
#include <string>

#include "cpptoml.h"
#include "feature_helper.hpp"
#include "helper.h"

/**
 * @brief
 *
 * VT_Int: literal integer constant
 * VT_Float: literal floating point constant
 * VT_String: literal string constant, with quotation marks as the beginning
 *            and end of the string
 * VT_IntList: literal integer constant array
 * VT_FloatList: literal floating point constant array
 * VT_StringList: literal string constant array
 * VT_Original_Feature: original feature, value of tffeature type, defined in
 * TensorFlow VT_Selected_Feature: named variable, value of tffeature type
 * VT_Anonymous_Feature: anonymous variable, value of tffeature type
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

// define function type
enum FunctionType {
  FT_Not_Defined = 0,
  FT_Simple_Func,
  FT_Unary_Func,
  FT_Binary_Func,
};

class ConfigureParameter {
 private:
  int index_;
  VariableType type_;

 private:
  int64_t int_;
  float float_;
  std::string str_;
  std::vector<int64_t> int_list_;
  std::vector<float> float_list_;
  std::vector<std::string> str_list_;

 public:
  ConfigureParameter() = delete;
  ConfigureParameter &operator=(const ConfigureParameter &p) {
    if (this == &p) {
      return *this;
    }
    this->type_ = p.type_;
    this->index_ = p.index_;

    switch (this->type_) {
      case VariableType::VT_Int:
        this->int_ = p.int_;
        break;
      case VariableType::VT_Float:
        this->float_ = p.float_;
        break;
      case VariableType::VT_String:
      case VariableType::VT_Origin_Feature:
      case VariableType::VT_Selected_Feature:
      case VariableType::VT_Anonymous_Feature:
        this->str_ = p.str_;
        break;
      case VariableType::VT_IntList:
        this->int_list_.assign(p.int_list_.begin(), p.int_list_.end());
        break;
      case VariableType::VT_FloatList:
        this->float_list_.assign(p.float_list_.begin(), p.float_list_.end());
        break;
      case VariableType::VT_StringList:
        this->str_list_.assign(p.str_list_.begin(), p.str_list_.end());
        break;
      default:
        break;
    }
    return *this;
  }

  ConfigureParameter(const ConfigureParameter &p) {
    this->type_ = p.type_;
    this->index_ = p.index_;

    switch (this->type_) {
      case VariableType::VT_Int:
        this->int_ = p.int_;
        break;
      case VariableType::VT_Float:
        this->float_ = p.float_;
        break;
      case VariableType::VT_String:
      case VariableType::VT_Origin_Feature:
      case VariableType::VT_Selected_Feature:
      case VariableType::VT_Anonymous_Feature:
        this->str_ = p.str_;
        break;
      case VariableType::VT_IntList:
        this->int_list_.assign(p.int_list_.begin(), p.int_list_.end());
        break;
      case VariableType::VT_FloatList:
        this->float_list_.assign(p.float_list_.begin(), p.float_list_.end());
        break;
      case VariableType::VT_StringList:
        this->str_list_.assign(p.str_list_.begin(), p.str_list_.end());
        break;
      default:
        break;
    }
  }

  // read toml config file and parse to ConfigureParameter
  ConfigureParameter(const std::shared_ptr<cpptoml::table> &table) {
    // check fields
    assert(table->contains("index") && table->contains("type") &&
           table->contains("data"));
    ParamsHelper params(table);
    this->index_ = params.get<int>("index");
    this->type_ = static_cast<VariableType>(params.get<int>("type"));
    switch (this->type_) {
      case VariableType::VT_Int:
        this->int_ = params.get<int64_t>("data");
        break;
      case VariableType::VT_IntList:
        for (const auto &v : params.get_array<int64_t>("data")) {
          this->int_list_.push_back(v);
        }
        break;
      case VariableType::VT_Float:
        this->float_ = params.get<double>("data");
        break;
      case VariableType::VT_FloatList:
        for (const auto &v : params.get_array<double>("data")) {
          this->float_list_.push_back(v);
        }
        break;
      case VariableType::VT_StringList:
        for (const auto &v : params.get_array<std::string>("data")) {
          this->str_list_.push_back(v);
        }
        break;

      case VariableType::VT_String:
        this->str_ = params.get<std::string>("data");
        break;
      case VariableType::VT_Origin_Feature:
      case VariableType::VT_Selected_Feature:
      case VariableType::VT_Anonymous_Feature:
        this->str_ = params.get<std::string>("data");
        break;
      default:
        break;
    }
  }

  ~ConfigureParameter() {}
  const int &get_index() const { return this->index_; }
  const VariableType &get_type() const { return this->type_; }
  const void *get() const {
    switch (this->type_) {
      case VariableType::VT_Int:
        return &this->int_;
      case VariableType::VT_Float:
        return &this->float_;
      case VariableType::VT_String:
      case VariableType::VT_Origin_Feature:
      case VariableType::VT_Selected_Feature:
      case VariableType::VT_Anonymous_Feature:
        return &this->str_;
      case VariableType::VT_IntList:
        return &this->int_list_;
      case VariableType::VT_FloatList:
        return &this->float_list_;
      case VariableType::VT_StringList:
        return &this->str_list_;
      default:
        return nullptr;
    }
  }
};

#define SharedParametersPtr std::shared_ptr<std::vector<ConfigureParameter>>

/**
 * @brief define function input type
 *
 */
enum FunctionInputType {
  FI_Not_Defined = 0,
  FI_Unary_S_1_L_1_Type,  // FI_Unary_S{ize}_[1-5]_L{ocation}_[1-5]_Type
  FI_Unary_S_2_L_1_Type,
  FI_Unary_S_2_L_2_Type,
  FI_Unary_S_3_L_1_Type,
  FI_Unary_S_3_L_2_Type,
  FI_Unary_S_3_L_3_Type,
  FI_Unary_S_4_L_1_Type,
  FI_Unary_S_4_L_2_Type,
  FI_Unary_S_4_L_3_Type,
  FI_Unary_S_4_L_4_Type,
  FI_Unary_S_5_L_1_Type,
  FI_Unary_S_5_L_2_Type,
  FI_Unary_S_5_L_3_Type,
  FI_Unary_S_5_L_4_Type,
  FI_Unary_S_5_L_5_Type,
  FI_Binary_S_2_L_1_2_Type,  // FI_Binary_S{ize}_[2-5]_L{ocation}_[1-5]_[1-5]_Type
  FI_Binary_S_3_L_1_2_Type,
  FI_Binary_S_3_L_1_3_Type,
  FI_Binary_S_3_L_2_3_Type,
  FI_Binary_S_4_L_1_2_Type,
  FI_Binary_S_4_L_1_3_Type,
  FI_Binary_S_4_L_1_4_Type,
  FI_Binary_S_4_L_2_3_Type,
  FI_Binary_S_4_L_2_4_Type,
  FI_Binary_S_4_L_3_4_Type,
  FI_Binary_S_5_L_1_2_Type,
  FI_Binary_S_5_L_1_3_Type,
  FI_Binary_S_5_L_1_4_Type,
  FI_Binary_S_5_L_1_5_Type,
  FI_Binary_S_5_L_2_3_Type,
  FI_Binary_S_5_L_2_4_Type,
  FI_Binary_S_5_L_2_5_Type,
  FI_Binary_S_5_L_3_4_Type,
  FI_Binary_S_5_L_3_5_Type,
  FI_Binary_S_5_L_4_5_Type
};

//配置中的operator
class ConfigureOperator {
 private:
  VariableType type_;
  FunctionInputType input_type_;
  FunctionType func_type_;
  std::string name_;
  std::string function_;
  SharedParametersPtr parameters_;

 public:
  ConfigureOperator() = delete;
  ConfigureOperator(const ConfigureOperator &o)
      : type_(o.type_),
        input_type_(o.input_type_),
        func_type_(o.func_type_),
        name_(o.name_),
        function_(o.function_),
        parameters_(o.parameters_) {}
  ConfigureOperator &operator=(const ConfigureOperator &o) {
    if (this == &o) {
      return *this;
    }
    this->name_ = o.name_;
    this->function_ = o.function_;
    this->parameters_ = o.parameters_;
    this->type_ = o.type_;
    this->func_type_ = o.func_type_;
    this->input_type_ = o.input_type_;
    return *this;
  }

  ConfigureOperator(const std::shared_ptr<cpptoml::table> &table) {
    assert(table->contains("name") && table->contains("func") &&
           table->contains("type") && table->contains("func_type") &&
           table->contains("input_type"));
    ParamsHelper params(table);
    this->name_ = params.get<std::string>("name");
    this->function_ = params.get<std::string>("func");
    this->type_ = static_cast<VariableType>(params.get<int>("type"));
    this->func_type_ = static_cast<FunctionType>(params.get<int>("func_type"));
    this->parameters_ = std::make_shared<std::vector<ConfigureParameter>>();
    this->input_type_ =
        static_cast<FunctionInputType>(params.get<int>("input_type"));

    if (table->contains("params")) {
      for (const auto &t : *table->get_table_array("params")) {
        this->parameters_->push_back(ConfigureParameter{t});
      }
    }
  }

  ~ConfigureOperator() {}
  const std::string &get_name() const { return this->name_; }
  const std::string &get_function() const { return this->function_; }
  const VariableType &get_type() const { return this->type_; }
  const FunctionType &get_function_type() const { return this->func_type_; }
  const FunctionInputType &get_input_type() const { return this->input_type_; }
  const SharedParametersPtr &get_parameters() const {
    return this->parameters_;
  }
};

#endif  // LUBAN_FEATURE_OPREATOR_CONFIGURE_HPP