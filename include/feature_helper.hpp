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

#ifndef LUBAN_FEATURE_HELPER_HPP
#define LUBAN_FEATURE_HELPER_HPP

#pragma once

// some help functions for feature.pb.h
#include <functional>
#include <type_traits>

#include "helper.h"

#define luban_is_int(T)                                       \
  (std::is_same_v<std::remove_pointer_t<T>, long long> ||     \
   std::is_same_v<std::remove_pointer_t<T>, int> ||           \
   std::is_same_v<std::remove_pointer_t<T>, long> ||          \
   std::is_same_v<std::remove_pointer_t<T>, unsigned long> || \
   std::is_same_v<std::remove_pointer_t<T>, unsigned long long>)

#define luban_is_int_array(T)                                              \
  (std::is_same_v<std::remove_pointer_t<T>, std::vector<long long>> ||     \
   std::is_same_v<std::remove_pointer_t<T>, std::vector<int>> ||           \
   std::is_same_v<std::remove_pointer_t<T>, std::vector<long>> ||          \
   std::is_same_v<std::remove_pointer_t<T>, std::vector<unsigned long>> || \
   std::is_same_v<std::remove_pointer_t<T>, std::vector<unsigned long long>>)

#define luban_is_float(T)                             \
  (std::is_same_v<std::remove_pointer_t<T>, float> || \
   std::is_same_v<std::remove_pointer_t<T>, double>)

#define luban_is_float_array(T)                                    \
  (std::is_same_v<std::remove_pointer_t<T>, std::vector<float>> || \
   std::is_same_v<std::remove_pointer_t<T>, std::vector<double>>)

#define luban_is_str(T)                                     \
  (std::is_same_v<std::remove_pointer_t<T>, std::string> || \
   std::is_same_v<std::remove_pointer_t<T>, std::string_view>)

#define luban_is_str_array(T)                                            \
  (std::is_same_v<std::remove_pointer_t<T>, std::vector<std::string>> || \
   std::is_same_v<std::remove_pointer_t<T>, std::vector<std::string_view>>)

#define luban_is_simple_type(T) \
  (luban_is_int(T) || luban_is_float(T) || luban_is_str(T))

#define luban_is_array_type(T) \
  (luban_is_int_array(T) || luban_is_float_array(T) || luban_is_str_array(T))

#define luban_is_number(T) (luban_is_float(T) || luban_is_int(T))

// fetch the data and tranform to array
// due to RVO, return vector is ok
template <typename T>
static std::vector<T> to_array(const SharedFeaturePtr &feature) {
  std::vector<T> ret;
  if constexpr (luban_is_number(T)) {
    assert(feature->has_int64_list() || feature->has_float_list());
    if (feature->has_int64_list()) {
      ret.reserve(feature->int64_list().value_size());
      for (int i = 0; i < feature->int64_list().value_size(); i++) {
        ret.push_back(static_cast<T>(feature->int64_list().value(i)));
      }
    } else if (feature->has_float_list()) {
      ret.reserve(feature->float_list().value_size());
      for (int i = 0; i < feature->float_list().value_size(); i++) {
        ret.push_back(static_cast<T>(feature->float_list().value(i)));
      }
    }
  } else if constexpr (luban_is_str(T)) {
    // int64_t, float type can cast to string
    if (feature->has_int64_list()) {
      ret.reserve(feature->int64_list().value_size());
      for (int i = 0; i < feature->int64_list().value_size(); i++) {
        ret.push_back(std::to_string(feature->int64_list().value(i)));
      }
    } else if (feature->has_float_list()) {
      ret.reserve(feature->float_list().value_size());
      for (int i = 0; i < feature->float_list().value_size(); i++) {
        ret.push_back(std::to_string(feature->float_list().value(i)));
      }
    } else {
      ret.reserve(feature->bytes_list().value_size());
      for (int i = 0; i < feature->bytes_list().value_size(); i++) {
        ret.push_back(feature->bytes_list().value(i));
      }
    }
  }
  return ret;
}

// add value to feature
template <typename T>
static void add_value(const SharedFeaturePtr &feature, const T &v) {
  if constexpr (luban_is_int(T)) {
    feature->mutable_int64_list()->add_value(static_cast<int64_t>(v));
    return;
  } else if constexpr (luban_is_float(T)) {
    feature->mutable_float_list()->add_value(static_cast<float>(v));
    return;
  } else if constexpr (luban_is_str(T)) {
    feature->mutable_bytes_list()->add_value(v);
    return;
  }
  throw std::runtime_error("type error");
}

// zero argument function call process
template <typename U>
static SimpleFunc get_simple_func(std::function<U()> func) {
  auto foo = [=]() -> SharedFeaturePtr {
    auto ret = std::make_shared<sample::Feature>();
    auto tmp = func();
    add_value<U>(ret, tmp);
    return ret;
  };
  return foo;
}

// one argument function call process
template <typename U, typename V>
static SharedFeaturePtr unary_func_call(const SharedFeaturePtr &feature,
                                        std::function<U(V &)> func) {
  if constexpr (luban_is_simple_type(V) && luban_is_simple_type(U)) {
    auto data = to_array<V>(feature);
    if (data.size() == 0) {
      return nullptr;
    }
    auto ret = std::make_shared<sample::Feature>();
    for (size_t i = 0; i < data.size(); i++) {
      auto tmp = func(data[i]);
      add_value<U>(ret, tmp);
    }
    return ret;
  } else if constexpr (luban_is_array_type(V) && luban_is_simple_type(U)) {
    auto data = to_array<typename V::value_type>(feature);
    if (data.size() == 0) {
      return nullptr;
    }
    auto ret = std::make_shared<sample::Feature>();
    auto tmp = func(data);
    add_value<U>(ret, tmp);
    return ret;
  } else if constexpr (luban_is_array_type(V) && luban_is_array_type(U)) {
    auto data = to_array<typename V::value_type>(feature);
    if (data.size() == 0) {
      return nullptr;
    }
    auto tmp = func(data);
    if (tmp.size() == 0) {
      return nullptr;
    }
    auto ret = std::make_shared<sample::Feature>();
    for (size_t i = 0; i < tmp.size(); i++) {
      add_value(ret, tmp[i]);
    }
    return ret;
  }
  return nullptr;
}

template <typename U, typename V>
static UnaryFunc get_unary_func(std::function<U(V &)> func) {
  return std::bind(unary_func_call<U, V>, std::placeholders::_1, func);
}

// two argument function call process
template <typename U, typename V, typename W>
static SharedFeaturePtr binary_func_call(const SharedFeaturePtr &featureA,
                                         const SharedFeaturePtr &featureB,
                                         std::function<U(V &, W &)> func) {
  if constexpr (luban_is_simple_type(U) && luban_is_simple_type(V) &&
                luban_is_simple_type(W)) {
    auto dataA = to_array<V>(featureA);
    auto dataB = to_array<W>(featureB);

    if (dataA.size() == 0 || dataB.size() == 0 ||
        dataA.size() != dataB.size()) {
      return nullptr;
    }
    auto ret = std::make_shared<sample::Feature>();
    for (size_t i = 0; i < dataA.size(); i++) {
      auto tmp = func(dataA[i], dataB[i]);
      add_value<U>(ret, tmp);
    }
    return ret;
  } else if constexpr (luban_is_simple_type(U) && luban_is_array_type(V) &&
                       luban_is_array_type(W)) {
    auto dataA = to_array<typename V::value_type>(featureA);
    auto dataB = to_array<typename W::value_type>(featureB);
    if (dataA.size() == 0 || dataB.size() == 0) {
      return nullptr;
    }
    auto ret = std::make_shared<sample::Feature>();
    auto tmp = func(dataA, dataB);
    add_value<U>(ret, tmp);
    return ret;
  } else if constexpr (luban_is_simple_type(U) && luban_is_array_type(V) &&
                       luban_is_simple_type(W)) {
    std::vector<typename V::value_type> dataA;
    to_array<typename V::value_type>(featureA, dataA);
    std::vector<W> dataB;
    to_array<W>(featureB, dataB);
    if (dataA.size() == 0 || dataB.size() == 0) {
      return nullptr;
    }
    auto ret = std::make_shared<sample::Feature>();
    for (size_t i = 0; i < dataB.size(); i++) {
      auto tmp = func(dataA, dataB[i]);
      add_value<U>(ret, tmp);
    }
    return ret;
  } else if constexpr (luban_is_simple_type(U) && luban_is_simple_type(V) &&
                       luban_is_array_type(W)) {
    auto dataA = to_array<V>(featureA);
    auto dataB = to_array<typename W::value_type>(featureB);
    if (dataA.size() == 0 || dataB.size() == 0) {
      return nullptr;
    }
    auto ret = std::make_shared<sample::Feature>();
    for (size_t i = 0; i < dataA.size(); i++) {
      auto tmp = func(dataA[i], dataB);
      add_value<U>(ret, tmp);
    }
    return ret;
  } else if constexpr (luban_is_array_type(U) && luban_is_array_type(V) &&
                       luban_is_array_type(W)) {
    auto dataA = to_array<typename V::value_type>(featureA);
    auto dataB = to_array<typename W::value_type>(featureB);
    if (dataA.size() == 0 || dataB.size() == 0) {
      return nullptr;
    }

    auto tmp = func(dataA, dataB);
    if (tmp.size() == 0) {
      return nullptr;
    }
    auto ret = std::make_shared<sample::Feature>();
    for (size_t i = 0; i < tmp.size(); i++) {
      add_value<typename U::value_type>(ret, tmp[i]);
    }
    return ret;
  }
  return nullptr;
}

template <typename U, typename V, typename W>
static BinaryFunc get_binary_func(std::function<U(V &, W &)> func) {
  return std::bind(binary_func_call<U, V, W>, std::placeholders::_1,
                   std::placeholders::_2, func);
}

#endif  // LUBAN_FEATURE_HELPER_HPP