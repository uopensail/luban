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

#ifndef LUBAN_OPERATOR_H
#define LUBAN_OPERATOR_H

#include "common.h"
#include "features.h"

namespace luban {

using Function =
    std::function<SharedParameter(const FunctionConfigure &conf, Features &)>;

class Wrapper {
 public:
  Wrapper() = delete;
  Wrapper(const FunctionConfigure &conf, Features &features);
  Wrapper(const Wrapper &w);
  Wrapper(const Wrapper &&w);
  ~Wrapper() = default;

  template <typename T>
  T *get() {
    if ((1 << m_index) & m_conf.flag) {
      // bit == 1, means variable
      const SharedParameter &ptr = m_feats[m_conf.variables[m_var]];
      if (ptr == nullptr) {
        m_var++;
        m_index++;
        return nullptr;
      }
      T *ret = std::get_if<T>(ptr.get());
      m_var++;
      m_index++;
      return ret;
    } else {
      T *ret = std::get_if<T>(m_conf.literals[m_lit].get());
      m_lit++;
      m_index++;
      return ret;
    }
  }

 private:
  int64_t m_index;
  int64_t m_lit;
  int64_t m_var;
  Features &m_feats;
  const FunctionConfigure &m_conf;
};

bool contain_nullptr();

template <typename T, typename... Args>
bool contain_nullptr(T *arg, Args *...args) {
  if constexpr (sizeof...(Args) == 0) {
    return arg == nullptr;
  } else {
    return contain_nullptr(args...);
  }
}

template <typename T0, typename... ArgsType>
SharedParameter safe_func_call(T0 (*func)(ArgsType &...), ArgsType *...args) {
  if (contain_nullptr(args...)) {
    return nullptr;
  }
  T0 ret = func((*args)...);
  SharedParameter p = std::make_shared<Parameter>();
  *p = ret;
  return p;
}

template <typename T0, typename... ArgsType>
static Function get_func_call(T0 (*func)(ArgsType &...)) {
  auto foo = [=](const FunctionConfigure &conf,
                 Features &features) -> SharedParameter {
    Wrapper w(conf, features);
    return safe_func_call<T0, ArgsType...>(func, w.get<ArgsType>()...);
  };
  return foo;
}

class Operator {
 public:
  Operator();
  Operator(const Operator &) = delete;
  Operator(const Operator &&) = delete;
  ~Operator() = default;
  void call(const FunctionConfigure &conf, Features &features);

 private:
  std::unordered_map<std::string, Function> m_funcs;
};

}  // namespace luban

#endif  // LUBAN_OPERATOR_H