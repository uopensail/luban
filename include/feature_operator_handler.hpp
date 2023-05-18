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

#ifndef LUBAN_FEATURE_OPERATOR_HANDLER_HPP
#define LUBAN_FEATURE_OPERATOR_HANDLER_HPP

#pragma once
#include "feature_helper.hpp"
#include "feature_operator_binary_handler.hpp"
#include "feature_operator_configure.hpp"
#include "feature_operator_unary_handler.hpp"

// warp functions have the same arguments
#define unary_function_wrapper(x)                               \
  static SharedFeaturePtr unary_function_wrapper_##x(           \
      RunTimeFeatures &feature, const ConfigureOperator &opr) { \
    return unary_call(feature, x, opr);                         \
  }

#define binary_function_wrapper(x)                              \
  static SharedFeaturePtr binary_function_wrapper_##x(          \
      RunTimeFeatures &feature, const ConfigureOperator &opr) { \
    return binary_call(feature, x, opr);                        \
  }

// add functions to global opr map
#define add_unary_func_to_global_oprs(global_oprs, x) \
  { global_oprs[#x] = unary_function_wrapper_##x; }

#define add_binary_func_to_global_oprs(global_oprs, x) \
  { global_oprs[#x] = binary_function_wrapper_##x; }

#endif  // LUBAN_FEATURE_OPERATOR_HANDLER_HPP