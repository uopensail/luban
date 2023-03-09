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

// namespace std {
// template <class T>
// struct remove_vector {
//   typedef T type;
// };
// template <class T>
// struct remove_vector<std::vector<T>> {
//   typedef T type;
// };
// template <class T>
// struct remove_vector<std::vector<T> *> {
//   typedef T type;
// };
// }  // namespace std

// fetch the data and tranform to array
template <typename T>
static void to_array(const SharedFeaturePtr &feature, std::vector<T> &ret) {
  if constexpr (luban_is_int(T)) {
    assert(feature->has_int64_list());
    ret.clear();
    ret.reserve(feature->int64_list().value_size());
    for (int i = 0; i < feature->int64_list().value_size(); i++) {
      ret.push_back(static_cast<T>(feature->int64_list().value(i)));
    }
    return;
  } else if constexpr (luban_is_float(T)) {
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
  } else if constexpr (luban_is_str(T)) {
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
// template <typename T>
// static T to_scalar(const SharedFeaturePtr &feature) {
//   if constexpr (is_int(T)) {
//     assert(feature->has_int64_list());
//     return static_cast<T>(feature->int64_list().value(0));
//   } else if constexpr (is_float(T)) {
//     // int64_t can cast as float
//     assert(feature->has_int64_list() || feature->has_float_list());
//     if (feature->has_int64_list()) {
//       return static_cast<T>(feature->int64_list().value(0));
//     } else {
//       return static_cast<T>(feature->float_list().value(0));
//     }
//   } else if constexpr (is_str(T)) {
//     assert(feature->has_bytes_list());
//     return feature->bytes_list().value(0);
//   }
//   throw std::runtime_error("feature value error");
// }

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

// one argument function call process
template <typename U, typename V>
static SharedFeaturePtr unary_func_call(const SharedFeaturePtr &feature,
                                        std::function<U(V &)> func) {
  if constexpr (luban_is_simple_type(V) && luban_is_simple_type(U)) {
    SharedFeaturePtr ret = std::make_shared<sample::Feature>();
    std::vector<V> data;
    to_array<V>(feature, data);
    if (data.size() == 0) {
      return nullptr;
    }
    for (size_t i = 0; i < data.size(); i++) {
      auto tmp = func(data[i]);
      add_value<U>(ret, tmp);
    }
    return ret;
  } else if constexpr (luban_is_array_type(V) && luban_is_simple_type(U)) {
    SharedFeaturePtr ret = std::make_shared<sample::Feature>();
    std::vector<typename V::value_type> data;
    to_array<typename V::value_type>(feature, data);
    if (data.size() == 0) {
      return nullptr;
    }
    auto tmp = func(data);
    add_value<U>(ret, tmp);
    return ret;
  } else if constexpr (luban_is_array_type(V) && luban_is_array_type(U)) {
    std::vector<typename V::value_type> data;
    to_array<typename V::value_type>(feature, data);
    if (data.size() == 0) {
      return nullptr;
    }
    auto tmp = func(data);
    if constexpr (std::is_pointer<U>::value) {
      if (tmp == nullptr) {
        return nullptr;
      } else if (tmp->size() == 0) {
        delete tmp;
        return nullptr;
      }
      SharedFeaturePtr ret = std::make_shared<sample::Feature>();
      for (size_t i = 0; i < tmp->size(); i++) {
        add_value<>(ret, tmp->at(i));
      }
      delete tmp;
      return ret;
    } else {
      if (tmp.size() == 0) {
        return nullptr;
      }
      SharedFeaturePtr ret = std::make_shared<sample::Feature>();
      for (size_t i = 0; i < tmp.size(); i++) {
        add_value<>(ret, tmp[i]);
      }
      return ret;
    }
  }
  return nullptr;
}

// two argument function call process
template <typename U, typename V, typename W>
static SharedFeaturePtr binary_func_call(const SharedFeaturePtr &featureA,
                                         const SharedFeaturePtr &featureB,
                                         std::function<U(V &, W &)> func) {
  if constexpr (luban_is_simple_type(U) && luban_is_simple_type(V) &&
                luban_is_simple_type(W)) {
    std::vector<V> dataA;
    std::vector<V> dataB;
    to_array<V>(featureA, dataA);
    to_array<W>(featureB, dataB);

    if (dataA.size() == 0 || dataB.size() == 0) {
      return nullptr;
    }
    SharedFeaturePtr ret = std::make_shared<sample::Feature>();
    for (size_t i = 0; i < dataA.size(); i++) {
      for (size_t j = 0; j < dataB.size(); j++) {
        auto tmp = func(dataA[i], dataB[j]);
        add_value<U>(ret, tmp);
      }
    }
    return ret;
  } else if constexpr (luban_is_simple_type(U) && luban_is_array_type(V) &&
                       luban_is_array_type(W)) {
    std::vector<typename V::value_type> dataA;
    to_array<typename V::value_type>(featureA, dataA);
    std::vector<typename W::value_type> dataB;
    to_array<typename W::value_type>(featureB, dataB);
    if (dataA.size() == 0 || dataB.size() == 0) {
      return nullptr;
    }
    SharedFeaturePtr ret = std::make_shared<sample::Feature>();
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
    SharedFeaturePtr ret = std::make_shared<sample::Feature>();
    for (size_t i = 0; i < dataB.size(); i++) {
      auto tmp = func(dataA, dataB[i]);
      add_value<U>(ret, tmp);
    }
    return ret;
  } else if constexpr (luban_is_simple_type(U) && luban_is_simple_type(V) &&
                       luban_is_array_type(W)) {
    std::vector<V> dataA;
    to_array<V>(featureA, dataA);
    std::vector<typename W::value_type> dataB;
    to_array<typename W::value_type>(featureB, dataB);
    to_array<typename W::value_type>(featureB, dataB);
    if (dataA.size() == 0 || dataB.size() == 0) {
      return nullptr;
    }
    SharedFeaturePtr ret = std::make_shared<sample::Feature>();
    for (size_t i = 0; i < dataA.size(); i++) {
      auto tmp = func(dataA[i], dataB);
      add_value<U>(ret, tmp);
    }
    return ret;
  } else if constexpr (luban_is_array_type(U) && luban_is_array_type(V) &&
                       luban_is_array_type(W)) {
    std::vector<typename V::value_type> dataA;
    to_array<typename V::value_type>(featureA, dataA);
    std::vector<typename W::value_type> dataB;
    to_array<typename W::value_type>(featureB, dataB);
    to_array<W>(featureB, dataB);
    if (dataA.size() == 0 || dataB.size() == 0) {
      return nullptr;
    }

    auto tmp = func(dataA, dataB);
    if constexpr (std::is_pointer<U>::value) {
      if (tmp->size() == 0) {
        delete tmp;
        return nullptr;
      }
      SharedFeaturePtr ret = std::make_shared<sample::Feature>();
      for (size_t i = 0; i < tmp->size(); i++) {
        add_value<>(ret, tmp->at(i));
      }
      delete tmp;
      return ret;
    } else {
      if (tmp.size() == 0) {
        return nullptr;
      }
      SharedFeaturePtr ret = std::make_shared<sample::Feature>();
      for (size_t i = 0; i < tmp.size(); i++) {
        add_value<typename U::value_type>(ret, tmp[i]);
      }
      return ret;
    }
  }
  return nullptr;
}

#endif  // LUBAN_FEATURE_HELPER_HPP