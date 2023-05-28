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

#ifndef LUBAN_FEATURE_OPERATOR_SIMPLE_HANDLER_HPP
#define LUBAN_FEATURE_OPERATOR_SIMPLE_HANDLER_HPP

#pragma once
#include <functional>

#include "configure.hpp"
#include "feature_helper.hpp"
#include "runtime.hpp"

class SimpleCaller {
public:
  SimpleCaller() = delete;
  SimpleCaller(SimpleFunc func) : func_(func) {}

  SimpleCaller &operator=(const SimpleCaller &caller) {
    if (this == &caller) {
      return *this;
    }
    this->func_ = caller.func_;
    return *this;
  }

  ~SimpleCaller() {}

  const SimpleFunc &get_func() const { return this->func_; }

private:
  SimpleFunc func_;
};

static SharedFeaturePtr do_simple_call(const SimpleCaller &caller) {
  auto func = caller.get_func();
  return func();
}

template <typename T0>
static std::shared_ptr<SimpleCaller> get_simple_caller(T0 (*func)()) {
  return std::make_shared<SimpleCaller>(get_simple_func<T0>(func));
}

#endif // LUBAN_FEATURE_OPERATOR_SIMPLE_HANDLER_HPP