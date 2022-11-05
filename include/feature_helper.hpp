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

#ifndef LUBAN_FEATURE_HELPER_HPP
#define LUBAN_FEATURE_HELPER_HPP

#pragma once

// some help functions for feature.pb.h
#include <functional>

#include "helper.h"

//类型判断
#define is_int(T)                                                 \
  (std::is_same_v<T, long long> || std::is_same_v<T, int> ||      \
   std::is_same_v<T, long> || std::is_same_v<T, unsigned long> || \
   std::is_same_v<T, unsigned long long>)

#define is_int_array(T)                             \
  (std::is_same_v<T, std::vector<long long>> ||     \
   std::is_same_v<T, std::vector<int>> ||           \
   std::is_same_v<T, std::vector<long>> ||          \
   std::is_same_v<T, std::vector<unsigned long>> || \
   std::is_same_v<T, std::vector<unsigned long long>>)

#define is_float(T) (std::is_same_v<T, float> || std::is_same_v<T, double>)

#define is_float_array(T)                   \
  (std::is_same_v<T, std::vector<float>> || \
   std::is_same_v<T, std::vector<double>>)

#define is_str(T) \
  (std::is_same_v<T, std::string> || std::is_same_v<T, std::string_view>)

#define is_str_array(T)                           \
  (std::is_same_v<T, std::vector<std::string>> || \
   std::is_same_v<T, std::vector<std::string_view>>)

#define is_simple_type(T) (is_int(T) || is_float(T) || is_str(T))

#define is_array_type(T) \
  (is_int_array(T) || is_float_array(T) || is_str_array(T))

// fetch the data and tranform to array
template <typename T>
static void to_array(const SharedFeaturePtr &feature, std::vector<T> &ret) {
  if constexpr (is_int(T)) {
    assert(feature->has_int64_list());
    ret.clear();
    ret.reserve(feature->int64_list().value_size());
    for (int i = 0; i < feature->int64_list().value_size(); i++) {
      ret.push_back(static_cast<T>(feature->int64_list().value(i)));
    }
    return;
  } else if constexpr (is_float(T)) {
    assert(feature->has_int64_list() || feature->has_float_list());
    // int64_t type can cast as float
    if (feature->has_int64_list()) {
      ret.clear();
      ret.reserve(feature->int64_list().value_size());
      for (int i = 0; i < feature->int64_list().value_size(); i++) {
        ret.push_back(static_cast<T>(feature->int64_list().value(i)));
      }
      return;
    } else if (feature->has_float_list()) {
      ret.clear();
      ret.reserve(feature->float_list().value_size());
      for (int i = 0; i < feature->float_list().value_size(); i++) {
        ret.push_back(static_cast<T>(feature->float_list().value(i)));
      }
      return;
    }
  } else if constexpr (is_str(T)) {
    assert(feature->has_bytes_list());
    ret.clear();
    ret.reserve(feature->bytes_list().value_size());
    for (int i = 0; i < feature->bytes_list().value_size(); i++) {
      ret.push_back(feature->bytes_list().value(i));
    }
    return;
  }
  throw std::runtime_error("feature value error");
}

// get the first data for list
template <typename T>
static T to_scalar(const SharedFeaturePtr &feature) {
  if constexpr (is_int(T)) {
    assert(feature->has_int64_list());
    return static_cast<T>(feature->int64_list().value(0));
  } else if constexpr (is_float(T)) {
    // int64_t can cast as float
    assert(feature->has_int64_list() || feature->has_float_list());
    if (feature->has_int64_list()) {
      return static_cast<T>(feature->int64_list().value(0));
    } else {
      return static_cast<T>(feature->float_list().value(0));
    }
  } else if constexpr (is_str(T)) {
    assert(feature->has_bytes_list());
    return feature->bytes_list().value(0);
  }
  throw std::runtime_error("feature value error");
}

// add value to feature
template <typename T>
static void add_value(const SharedFeaturePtr &feature, const T &v) {
  if constexpr (is_int(T)) {
    feature->mutable_int64_list()->add_value(static_cast<int64_t>(v));
    return;
  } else if constexpr (is_float(T)) {
    feature->mutable_float_list()->add_value(static_cast<float>(v));
    return;
  } else if constexpr (is_str(T)) {
    feature->mutable_bytes_list()->add_value(v);
    return;
  }
  throw std::runtime_error("value type error");
}

//处理feature
template <typename U, typename V>
static SharedFeaturePtr unary_func_call(const SharedFeaturePtr &feature,
                                        std::function<U(V &)> func) {
  //输入是单值, 输出是单值
  if constexpr (is_simple_type(V) && is_simple_type(U)) {
    SharedFeaturePtr ret = std::make_shared<tensorflow::Feature>();
    std::vector<V> data;
    to_array<V>(feature, data);
    for (size_t i = 0; i < data.size(); i++) {
      auto tmp = func(data[i]);
      add_value<U>(ret, tmp);
    }
    return ret;
  } else if constexpr (is_array_type(V) && is_simple_type(U)) {
    SharedFeaturePtr ret = std::make_shared<tensorflow::Feature>();
    std::vector<typename V::value_type> data;
    to_array<typename V::value_type>(feature, data);
    auto tmp = func(data);
    add_value<U>(ret, tmp);
    return ret;
  } else if constexpr (is_array_type(V) && is_array_type(U)) {
    SharedFeaturePtr ret = std::make_shared<tensorflow::Feature>();
    std::vector<typename V::value_type> data;
    to_array<typename V::value_type>(feature, data);
    auto tmp = func(data);
    for (size_t i = 0; i < tmp.size(); i++) {
      add_value<typename U::value_type>(ret, tmp);
    }
    return ret;
  }
  return nullptr;
}

//处理feature
template <typename U, typename V, typename W>
static SharedFeaturePtr binary_func_call(const SharedFeaturePtr &featureA,
                                         const SharedFeaturePtr &featureB,
                                         std::function<U(V &, W &)> func) {
  //输入是单值, 输出是单值
  if constexpr (is_simple_type(U) && is_simple_type(V) && is_simple_type(W)) {
    std::vector<V> dataA;
    std::vector<V> dataB;
    to_array<V>(featureA, dataA);
    to_array<W>(featureB, dataB);

    if (dataA.size() == 0 || dataB.size() == 0) {
      return nullptr;
    }
    SharedFeaturePtr ret = std::make_shared<tensorflow::Feature>();
    for (size_t i = 0; i < dataA.size(); i++) {
      for (size_t j = 0; j < dataB.size(); j++) {
        auto tmp = func(dataA[i], dataB[j]);
        add_value<U>(ret, tmp);
      }
    }
    return ret;
  } else if constexpr (is_simple_type(U) && is_array_type(V) &&
                       is_array_type(W)) {
    std::vector<typename V::value_type> dataA;
    to_array<typename V::value_type>(featureA, dataA);
    std::vector<typename W::value_type> dataB;
    to_array<typename W::value_type>(featureB, dataB);
    if (dataA.size() == 0 || dataB.size() == 0) {
      return nullptr;
    }
    SharedFeaturePtr ret = std::make_shared<tensorflow::Feature>();
    auto tmp = func(dataA, dataB);
    add_value<U>(ret, tmp);

    return ret;
  } else if constexpr (is_simple_type(U) && is_array_type(V) &&
                       is_simple_type(W)) {
    std::vector<typename V::value_type> dataA;
    to_array<typename V::value_type>(featureA, dataA);
    std::vector<W> dataB;
    to_array<W>(featureB, dataB);
    if (dataA.size() == 0 || dataB.size() == 0) {
      return nullptr;
    }
    SharedFeaturePtr ret = std::make_shared<tensorflow::Feature>();
    for (size_t i = 0; i < dataB.size(); i++) {
      auto tmp = func(dataA, dataB[i]);
      add_value<U>(ret, tmp);
    }
    return ret;
  } else if constexpr (is_simple_type(U) && is_simple_type(V) &&
                       is_array_type(W)) {
    std::vector<V> dataA;
    to_array<V>(featureA, dataA);
    std::vector<typename W::value_type> dataB;
    to_array<typename W::value_type>(featureB, dataB);
    to_array<typename W::value_type>(featureB, dataB);
    if (dataA.size() == 0 || dataB.size() == 0) {
      return nullptr;
    }
    SharedFeaturePtr ret = std::make_shared<tensorflow::Feature>();
    for (size_t i = 0; i < dataA.size(); i++) {
      auto tmp = func(dataA[i], dataB);
      add_value<U>(ret, tmp);
    }
    return ret;
  } else if constexpr (is_array_type(U) && is_array_type(V) &&
                       is_array_type(W)) {
    std::vector<typename V::value_type> dataA;
    to_array<typename V::value_type>(featureA, dataA);
    std::vector<typename W::value_type> dataB;
    to_array<typename W::value_type>(featureB, dataB);
    to_array<W>(featureB, dataB);
    if (dataA.size() == 0 || dataB.size() == 0) {
      return nullptr;
    }
    auto tmp = func(dataA, dataB);
    if (tmp.size() == 0) {
      return nullptr;
    }
    SharedFeaturePtr ret = std::make_shared<tensorflow::Feature>();

    for (size_t i = 0; i < tmp.size(); i++) {
      add_value<typename U::value_type>(ret, tmp);
    }
    return ret;
  }
  return nullptr;
}

#endif  // LUBAN_FEATURE_HELPER_HPP