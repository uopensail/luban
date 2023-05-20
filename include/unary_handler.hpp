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

#ifndef LUBAN_FEATURE_OPERATOR_UNARY_HANDLER_HPP
#define LUBAN_FEATURE_OPERATOR_UNARY_HANDLER_HPP

#pragma once
#include <functional>

#include "configure.hpp"
#include "feature_helper.hpp"
#include "runtime.hpp"

class UnaryCaller {
public:
  UnaryCaller() = delete;
  UnaryCaller(UnaryFunc func, const ConfigureParameter &arg)
      : func_(func), arg_(arg) {}

  UnaryCaller &operator=(const UnaryCaller &caller) {
    if (this == &caller) {
      return *this;
    }
    this->func_ = caller.func_;
    this->arg_ = caller.arg_;
    return *this;
  }

  ~UnaryCaller() {}

  const UnaryFunc &get_func() const { return this->func_; }
  const ConfigureParameter &get_arg() const { return this->arg_; }

private:
  UnaryFunc func_;
  ConfigureParameter arg_;
};

static SharedFeaturePtr do_unary_call(const UnaryCaller &caller,
                                      RunTimeFeatures &features) {
  auto feature = features.get(caller.get_arg());
  if (feature == nullptr) {
    return nullptr;
  }
  auto func = caller.get_func();
  return func(feature);
}

template <class T0, class T1>
std::function<T0(T1 &)> unary_call_1_1(T0 (*func)(T1 &),
                                       const SharedParametersPtr &params) {
  return func;
}

template <class T0, class T1, class T2>
std::function<T0(T1 &)> unary_call_2_1(T0 (*func)(T1 &, T2 &),
                                       const SharedParametersPtr &params) {
  return std::bind(func, std::placeholders::_1, *(T2 *)params->at(1).get());
}

template <class T0, class T1, class T2>
std::function<T0(T2 &)> unary_call_2_2(T0 (*func)(T1 &, T2 &),
                                       const SharedParametersPtr &params) {
  return std::bind(func, *(T1 *)params->at(0).get(), std::placeholders::_1);
}

template <class T0, class T1, class T2, class T3>
std::function<T0(T1 &)> unary_call_3_1(T0 (*func)(T1 &, T2 &, T3 &),
                                       const SharedParametersPtr &params) {
  return std::bind(func, std::placeholders::_1, *(T2 *)params->at(1).get(),
                   *(T3 *)params->at(2).get());
}

template <class T0, class T1, class T2, class T3>
std::function<T0(T2 &)> unary_call_3_2(T0 (*func)(T1 &, T2 &, T3 &),
                                       const SharedParametersPtr &params) {
  return std::bind(func, *(T1 *)params->at(0).get(), std::placeholders::_1,
                   *(T3 *)params->at(2).get());
}

template <class T0, class T1, class T2, class T3>
std::function<T0(T3 &)> unary_call_3_3(T0 (*func)(T1 &, T2 &, T3 &),
                                       const SharedParametersPtr &params) {
  return std::bind(func, *(T1 *)params->at(0).get(), *(T2 *)params->at(1).get(),
                   std::placeholders::_1);
}

template <class T0, class T1, class T2, class T3, class T4>
std::function<T0(T1 &)> unary_call_4_1(T0 (*func)(T1 &, T2 &, T3 &, T4 &),
                                       const SharedParametersPtr &params) {
  return std::bind(func, std::placeholders::_1, *(T2 *)params->at(1).get(),
                   *(T3 *)params->at(2).get(), *(T4 *)params->at(3).get());
}

template <class T0, class T1, class T2, class T3, class T4>
std::function<T0(T2 &)> unary_call_4_2(T0 (*func)(T1 &, T2 &, T3 &, T4 &),
                                       const SharedParametersPtr &params) {
  return std::bind(func, *(T1 *)params->at(0).get(), std::placeholders::_1,
                   *(T3 *)params->at(2).get(), *(T4 *)params->at(3).get());
}

template <class T0, class T1, class T2, class T3, class T4>
std::function<T0(T3 &)> unary_call_4_3(T0 (*func)(T1 &, T2 &, T3 &, T4 &),
                                       const SharedParametersPtr &params) {
  return std::bind(func, *(T1 *)params->at(0).get(), *(T2 *)params->at(1).get(),
                   std::placeholders::_1, *(T4 *)params->at(3).get());
}

template <class T0, class T1, class T2, class T3, class T4>
std::function<T0(T4 &)> unary_call_4_4(T0 (*func)(T1 &, T2 &, T3 &, T4 &),
                                       const SharedParametersPtr &params) {
  return std::bind(func, *(T1 *)params->at(0).get(), *(T2 *)params->at(1).get(),
                   *(T3 *)params->at(2).get(), std::placeholders::_1);
}

template <class T0, class T1, class T2, class T3, class T4, class T5>
std::function<T0(T1 &)> unary_call_5_1(T0 (*func)(T1 &, T2 &, T3 &, T4 &, T5 &),
                                       const SharedParametersPtr &params) {
  return std::bind(func, std::placeholders::_1, *(T2 *)params->at(1).get(),
                   *(T3 *)params->at(2).get(), *(T4 *)params->at(3).get(),
                   *(T5 *)params->at(4).get());
}

template <class T0, class T1, class T2, class T3, class T4, class T5>
std::function<T0(T2 &)> unary_call_5_2(T0 (*func)(T1 &, T2 &, T3 &, T4 &, T5 &),
                                       const SharedParametersPtr &params) {
  return std::bind(func, *(T1 *)params->at(0).get(), std::placeholders::_1,
                   *(T3 *)params->at(2).get(), *(T4 *)params->at(3).get(),
                   *(T5 *)params->at(4).get());
}

template <class T0, class T1, class T2, class T3, class T4, class T5>
std::function<T0(T3 &)> unary_call_5_3(T0 (*func)(T1 &, T2 &, T3 &, T4 &, T5 &),
                                       const SharedParametersPtr &params) {
  return std::bind(func, *(T1 *)params->at(0).get(), *(T2 *)params->at(1).get(),
                   std::placeholders::_1, *(T4 *)params->at(3).get(),
                   *(T5 *)params->at(4).get());
}

template <class T0, class T1, class T2, class T3, class T4, class T5>
std::function<T0(T4 &)> unary_call_5_4(T0 (*func)(T1 &, T2 &, T3 &, T4 &, T5 &),
                                       const SharedParametersPtr &params) {
  return std::bind(func, *(T1 *)params->at(0).get(), *(T2 *)params->at(1).get(),
                   *(T3 *)params->at(2).get(), std::placeholders::_1,
                   *(T5 *)params->at(4).get());
}

template <class T0, class T1, class T2, class T3, class T4, class T5>
std::function<T0(T5 &)> unary_call_5_5(T0 (*func)(T1 &, T2 &, T3 &, T4 &, T5 &),
                                       const SharedParametersPtr &params) {
  return std::bind(func, *(T1 *)params->at(0).get(), *(T2 *)params->at(1).get(),
                   *(T3 *)params->at(2).get(), *(T4 *)params->at(3).get(),
                   std::placeholders::_1);
}

template <typename T0, typename... ArgsType>
static std::shared_ptr<UnaryCaller>
get_unary_caller(T0 (*func)(ArgsType &...), const ConfigureOperator &opr) {
  if constexpr (sizeof...(ArgsType) > 5) {
    throw std::runtime_error("len(argvs) > 5");
  } else if constexpr (sizeof...(ArgsType) == 0) {
    throw std::runtime_error("len(argvs) == 0");
  } else if constexpr (sizeof...(ArgsType) == 1) {
    const FunctionInputType &type = opr.get_input_type();
    const SharedParametersPtr &params = opr.get_parameters();
    assert(params->size() == 1);
    assert(type == FunctionInputType::FI_Unary_S_1_L_1_Type);
    auto myfunc = unary_call_1_1(func, params);
    return std::make_shared<UnaryCaller>(get_unary_func(myfunc), params->at(0));
  } else if constexpr (sizeof...(ArgsType) == 2) {
    const FunctionInputType &type = opr.get_input_type();
    const SharedParametersPtr &params = opr.get_parameters();
    assert(params->size() == 2);
    if (type == FunctionInputType::FI_Unary_S_2_L_1_Type) {
      auto myfunc = unary_call_2_1(func, params);
      return std::make_shared<UnaryCaller>(get_unary_func(myfunc),
                                           params->at(0));
    } else if (type == FunctionInputType::FI_Unary_S_2_L_2_Type) {
      auto myfunc = unary_call_2_2(func, params);
      return std::make_shared<UnaryCaller>(get_unary_func(myfunc),
                                           params->at(1));
    }
  } else if constexpr (sizeof...(ArgsType) == 3) {
    const FunctionInputType &type = opr.get_input_type();
    const SharedParametersPtr &params = opr.get_parameters();
    assert(params->size() == 3);
    if (type == FunctionInputType::FI_Unary_S_3_L_1_Type) {
      auto myfunc = unary_call_3_1(func, params);
      return std::make_shared<UnaryCaller>(get_unary_func(myfunc),
                                           params->at(0));
    } else if (type == FunctionInputType::FI_Unary_S_3_L_2_Type) {
      auto myfunc = unary_call_3_2(func, params);
      return std::make_shared<UnaryCaller>(get_unary_func(myfunc),
                                           params->at(1));
    } else if (type == FunctionInputType::FI_Unary_S_3_L_3_Type) {
      auto myfunc = unary_call_3_3(func, params);
      return std::make_shared<UnaryCaller>(get_unary_func(myfunc),
                                           params->at(2));
    }
  } else if constexpr (sizeof...(ArgsType) == 4) {
    const FunctionInputType &type = opr.get_input_type();
    const SharedParametersPtr &params = opr.get_parameters();
    assert(params->size() == 4);
    if (type == FunctionInputType::FI_Unary_S_4_L_1_Type) {
      auto myfunc = unary_call_4_1(func, params);
      return std::make_shared<UnaryCaller>(get_unary_func(myfunc),
                                           params->at(0));
    } else if (type == FunctionInputType::FI_Unary_S_4_L_2_Type) {
      auto myfunc = unary_call_4_2(func, params);
      return std::make_shared<UnaryCaller>(get_unary_func(myfunc),
                                           params->at(1));
    } else if (type == FunctionInputType::FI_Unary_S_4_L_3_Type) {
      auto myfunc = unary_call_4_3(func, params);
      return std::make_shared<UnaryCaller>(get_unary_func(myfunc),
                                           params->at(2));
    } else if (type == FunctionInputType::FI_Unary_S_4_L_4_Type) {
      auto myfunc = unary_call_4_4(func, params);
      return std::make_shared<UnaryCaller>(get_unary_func(myfunc),
                                           params->at(3));
    }
  } else if constexpr (sizeof...(ArgsType) == 5) {
    const FunctionInputType &type = opr.get_input_type();
    const SharedParametersPtr &params = opr.get_parameters();
    assert(params->size() == 5);
    if (type == FunctionInputType::FI_Unary_S_5_L_1_Type) {
      auto myfunc = unary_call_5_1(func, params);
      return std::make_shared<UnaryCaller>(get_unary_func(myfunc),
                                           params->at(0));
    } else if (type == FunctionInputType::FI_Unary_S_5_L_2_Type) {
      auto myfunc = unary_call_5_2(func, params);
      return std::make_shared<UnaryCaller>(get_unary_func(myfunc),
                                           params->at(1));
    } else if (type == FunctionInputType::FI_Unary_S_5_L_3_Type) {
      auto myfunc = unary_call_5_3(func, params);
      return std::make_shared<UnaryCaller>(get_unary_func(myfunc),
                                           params->at(2));
    } else if (type == FunctionInputType::FI_Unary_S_5_L_4_Type) {
      auto myfunc = unary_call_5_4(func, params);
      return std::make_shared<UnaryCaller>(get_unary_func(myfunc),
                                           params->at(3));
    } else if (type == FunctionInputType::FI_Unary_S_5_L_5_Type) {
      auto myfunc = unary_call_5_5(func, params);
      return std::make_shared<UnaryCaller>(get_unary_func(myfunc),
                                           params->at(4));
    }
  }
  return nullptr;
}

#endif // LUBAN_FEATURE_OPERATOR_UNARY_HANDLER_HPP