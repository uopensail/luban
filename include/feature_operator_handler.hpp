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

#ifndef LUBAN_FEATURE_OPERATOR_HANDLER_HPP
#define LUBAN_FEATURE_OPERATOR_HANDLER_HPP

#pragma once
#include "feature_helper.hpp"
#include "feature_operator_configure.hpp"
#include "feature_operator_runtime.hpp"

//函数参数的辅助类
class FunctionParameterHelper {
private:
  std::vector<RunTimeParameter> &params_;
  int index_;

public:
  FunctionParameterHelper() = delete;
  FunctionParameterHelper(std::vector<RunTimeParameter> &params)
      : params_(params), index_(0) {}
  ~FunctionParameterHelper() {}
  void *get() { return (void *)this->params_[this->index_++].get(); }
};

//调用map算子函数
template <typename T, typename U>
static SharedFeaturePtr unary_map_call(const SharedFeaturePtr &feature,
                                       T (*func)(U &),
                                       std::vector<RunTimeParameter> &params) {
  return unary_map_func<T, U>(feature, func);
}

template <typename T, typename U, typename... ArgsType>
static SharedFeaturePtr unary_map_call(const SharedFeaturePtr &feature,
                                       T (*func)(U &, ArgsType &...),
                                       std::vector<RunTimeParameter> &params) {
  FunctionParameterHelper fp(params);

  auto myfunc = [&](U &u) -> T { return func(u, *(ArgsType *)fp.get()...); };
  // auto myfunc =std::bind(func, std::placeholders::_1, *(ArgsType
  // *)fp.get()...);
  return unary_map_func<T, U>(feature, myfunc);
}

//调用agg算子函数
template <typename T, typename U>
static SharedFeaturePtr
unary_agg_call(const SharedFeaturePtr &feature,
               std::vector<T> *(*func)(std::vector<U> &),
               std::vector<RunTimeParameter> &params) {
  return unary_agg_func<T, U>(feature, func);
}

template <typename T, typename U, typename... ArgsType>
static SharedFeaturePtr unary_agg_call(const SharedFeaturePtr &feature,
                                       std::vector<T> *(*func)(std::vector<U> &,
                                                               ArgsType &...),
                                       std::vector<RunTimeParameter> &params) {
  FunctionParameterHelper fp(params);
  auto myfunc = [&](std::vector<U> &u) -> std::vector<T> * {
    return func(u, *(ArgsType *)fp.get()...);
  };

  return unary_agg_func<T, U>(feature, myfunc);
}

//调用cross算子函数
// template <typename T, typename U, typename W>
// static SharedFeaturePtr
// cartesian_cross_call(const SharedFeaturePtr &featureA,
//                      const SharedFeaturePtr &featureB, T (*func)(U &, W &),
//                      std::vector<RunTimeParameter> &params) {
//   return cartesian_cross_func<T, U, W>(featureA, featureB, func);
// }

// template <typename T, typename U, typename W, typename... ArgsType>
// static SharedFeaturePtr cartesian_cross_call(
//     const SharedFeaturePtr &featureA, const SharedFeaturePtr &featureB,
//     T (*func)(U &, W &, ArgsType &...), std::vector<RunTimeParameter>
//     &params) {
//   FunctionParameterHelper fp(params);
//   // use lamdba, then compile knows how to optimize
//   auto myfunc = [&](U &u, W &w) -> T {
//     return func(u, w, *(ArgsType *)fp.get()...);
//   };
//   //   auto myfunc = std::bind(func, std::placeholders::_1,
//   //   std::placeholders::_2,
//   //                           *(ArgsType *)fp.get()...);
//   return cartesian_cross_func<T, U, W>(featureA, featureB, myfunc);
// }

//调用hadamard_map算子函数
template <typename T, typename U, typename W>
static SharedFeaturePtr
hadamard_map_call(const SharedFeaturePtr &featureA,
                  const SharedFeaturePtr &featureB, T (*func)(U &, W &),
                  std::vector<RunTimeParameter> &params) {
  return hadamard_map_func<T, U, W>(featureA, featureB, func);
}

template <typename T, typename U, typename W, typename... ArgsType>
static SharedFeaturePtr hadamard_map_call(
    const SharedFeaturePtr &featureA, const SharedFeaturePtr &featureB,
    T (*func)(U &, W &, ArgsType &...), std::vector<RunTimeParameter> &params) {
  FunctionParameterHelper fp(params);
  auto myfunc = [&](U &u, W &w) -> T {
    return func(u, w, *(ArgsType *)fp.get()...);
  };

  //   auto myfunc = std::bind(func, std::placeholders::_1,
  //   std::placeholders::_2,
  //                           *(ArgsType *)fp.get()...);
  return hadamard_map_func<T, U, W>(featureA, featureB, myfunc);
}

//调用hadamard_map算子函数
template <typename T, typename U, typename W>
static SharedFeaturePtr
hadamard_agg_call(const SharedFeaturePtr &featureA,
                  const SharedFeaturePtr &featureB,
                  std::vector<T> *(*func)(std::vector<U> &, std::vector<W> &),
                  std::vector<RunTimeParameter> &params) {
  return hadamard_agg_func<T, U, W>(featureA, featureB, func);
}

template <typename T, typename U, typename W, typename... ArgsType>
static SharedFeaturePtr hadamard_agg_call(
    const SharedFeaturePtr &featureA, const SharedFeaturePtr &featureB,
    std::vector<T> *(*func)(std::vector<U> &, std::vector<W> &, ArgsType...),
    std::vector<RunTimeParameter> &params) {
  FunctionParameterHelper fp(params);
  auto myfunc = [&](std::vector<U> &u, std::vector<W> &w) -> std::vector<T> * {
    return func(u, w, *(ArgsType *)fp.get()...);
  };

  return hadamard_map_func<T, U, W>(featureA, featureB, myfunc);
}

//函数外面做一下封装, 为了统一放在map中个进行管理
#define unary_map_function_wrapper(x)                                          \
  static SharedFeaturePtr unary_map_function_wrapper_##x(                      \
      const SharedFeaturePtr &feature,                                         \
      std::vector<RunTimeParameter> &params) {                                 \
    return unary_map_call(feature, x, params);                                 \
  }

#define unary_agg_function_wrapper(x)                                          \
  static SharedFeaturePtr unary_agg_function_wrapper_##x(                      \
      const SharedFeaturePtr &feature,                                         \
      std::vector<RunTimeParameter> &params) {                                 \
    return unary_agg_call(feature, x, params);                                 \
  }

// #define cartesian_cross_function_wrapper(x)                                    \
//   static SharedFeaturePtr cartesian_cross_function_wrapper_##x(                \
//       const SharedFeaturePtr &featureA, const SharedFeaturePtr &featureB,      \
//       std::vector<RunTimeParameter> &params) {                                 \
//     return cartesian_cross_call(featureA, featureB, x, params);                \
//   }

#define hadamard_map_function_wrapper(x)                                       \
  static SharedFeaturePtr hadamard_map_function_wrapper_##x(                   \
      const SharedFeaturePtr &featureA, const SharedFeaturePtr &featureB,      \
      std::vector<RunTimeParameter> &params) {                                 \
    return hadamard_map_call(featureA, featureB, x, params);                   \
  }

#define hadamard_agg_function_wrapper(x)                                       \
  static SharedFeaturePtr hadamard_agg_function_wrapper_##x(                   \
      const SharedFeaturePtr &featureA, const SharedFeaturePtr &featureB,      \
      std::vector<RunTimeParameter> &params) {                                 \
    return hadamard_agg_call(featureA, featureB, x, params);                   \
  }

//把封装的函数添加到全局map中
#define add_unary_map_func_to_global_oprs(global_oprs, x)                      \
  { global_oprs[#x] = unary_map_function_wrapper_##x; }

#define add_unary_agg_func_to_global_oprs(global_oprs, x)                      \
  { global_oprs[#x] = unary_agg_function_wrapper_##x; }
// #define add_cartesian_cross_func_to_global_oprs(global_oprs, x)                \
//   { global_oprs[#x] = cartesian_cross_function_wrapper_##x; }
#define add_hadamard_map_func_to_global_oprs(global_oprs, x)                   \
  { global_oprs[#x] = hadamard_map_function_wrapper_##x; }

#define add_hadamard_agg_func_to_global_oprs(global_oprs, x)                   \
  { global_oprs[#x] = hadamard_agg_function_wrapper_##x; }
#endif // LUBAN_FEATURE_OPERATOR_HANDLER_HPP