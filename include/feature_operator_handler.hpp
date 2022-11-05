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

#ifndef LUBAN_FEATURE_OPERATOR_HANDLER_HPP
#define LUBAN_FEATURE_OPERATOR_HANDLER_HPP

#pragma once
#include "feature_helper.hpp"
#include "feature_operator_binary_handler.hpp"
#include "feature_operator_configure.hpp"
#include "feature_operator_unary_handler.hpp"

//函数外面做一下封装, 为了统一放在map中个进行管理
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

//把封装的函数添加到全局map中
#define add_unary_func_to_global_oprs(global_oprs, x) \
  { global_oprs[#x] = unary_function_wrapper_##x; }

#define add_binary_func_to_global_oprs(global_oprs, x) \
  { global_oprs[#x] = binary_function_wrapper_##x; }

#endif  // LUBAN_FEATURE_OPERATOR_HANDLER_HPP