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

#ifndef LUBAN_FEATURE_OPERATOR_BINARY_HANDLER_HPP
#define LUBAN_FEATURE_OPERATOR_BINARY_HANDLER_HPP

#pragma once
#include <functional>

#include "configure.hpp"
#include "feature_helper.hpp"
#include "runtime.hpp"

class BinaryCaller {
public:
  BinaryCaller() = delete;
  BinaryCaller(BinaryFunc func, const std::string &arg1,
               const std::string &arg2)
      : func_(func), arg1_(arg1), arg2_(arg2) {}

  BinaryCaller &operator=(const BinaryCaller &caller) {
    if (this == &caller) {
      return *this;
    }
    this->func_ = caller.func_;
    this->arg1_ = caller.arg1_;
    this->arg2_ = caller.arg2_;
    return *this;
  }

  ~BinaryCaller() {}

  const BinaryFunc &get_func() const { return this->func_; }
  const std::string &get_arg1() const { return this->arg1_; }
  const std::string &get_arg2() const { return this->arg2_; }

private:
  BinaryFunc func_;
  std::string arg1_;
  std::string arg2_;
};

static SharedFeaturePtr do_binary_call(const BinaryCaller &caller,
                                       RunTimeFeatures &features) {
  auto feature1 = features.get(caller.get_arg1());
  auto feature2 = features.get(caller.get_arg2());
  if (feature1 == nullptr || feature2 == nullptr) {
    return nullptr;
  }
  auto func = caller.get_func();
  return func(feature1, feature2);
}

template <class T0, class T1, class T2>
std::function<T0(T1 &, T2 &)>
binary_call_2_1_2(T0 (*func)(T1 &, T2 &), const SharedArgumentsPtr &params) {
  return func;
}

template <class T0, class T1, class T2, class T3>
std::function<T0(T1 &, T2 &)>
binary_call_3_1_2(T0 (*func)(T1 &, T2 &, T3 &),
                  const SharedArgumentsPtr &params) {
  return std::bind(func, std::placeholders::_1, std::placeholders::_2,
                   *(T3 *)params->at(2).get());
}

template <class T0, class T1, class T2, class T3>
std::function<T0(T1 &, T3 &)>
binary_call_3_1_3(T0 (*func)(T1 &, T2 &, T3 &),
                  const SharedArgumentsPtr &params) {
  return std::bind(func, std::placeholders::_1, *(T2 *)params->at(1).get(),
                   std::placeholders::_2);
}

template <class T0, class T1, class T2, class T3>
std::function<T0(T2 &, T3 &)>
binary_call_3_2_3(T0 (*func)(T1 &, T2 &, T3 &),
                  const SharedArgumentsPtr &params) {
  return std::bind(func, *(T1 *)params->at(0).get(), std::placeholders::_1,
                   std::placeholders::_2);
}

template <class T0, class T1, class T2, class T3, class T4>
std::function<T0(T1 &, T2 &)>
binary_call_4_1_2(T0 (*func)(T1 &, T2 &, T3 &, T4 &),
                  const SharedArgumentsPtr &params) {
  return std::bind(func, std::placeholders::_1, std::placeholders::_2,
                   *(T3 *)params->at(2).get(), *(T4 *)params->at(3).get());
}

template <class T0, class T1, class T2, class T3, class T4>
std::function<T0(T1 &, T3 &)>
binary_call_4_1_3(T0 (*func)(T1 &, T2 &, T3 &, T4 &),
                  const SharedArgumentsPtr &params) {
  return std::bind(func, std::placeholders::_1, *(T2 *)params->at(1).get(),
                   std::placeholders::_2, *(T4 *)params->at(3).get());
}

template <class T0, class T1, class T2, class T3, class T4>
std::function<T0(T1 &, T4 &)>
binary_call_4_1_4(T0 (*func)(T1 &, T2 &, T3 &, T4 &),
                  const SharedArgumentsPtr &params) {
  return std::bind(func, std::placeholders::_1, *(T2 *)params->at(1).get(),
                   *(T3 *)params->at(2).get(), std::placeholders::_2);
}

template <class T0, class T1, class T2, class T3, class T4>
std::function<T0(T2 &, T3 &)>
binary_call_4_2_3(T0 (*func)(T1 &, T2 &, T3 &, T4 &),
                  const SharedArgumentsPtr &params) {
  return std::bind(func, *(T1 *)params->at(0).get(), std::placeholders::_1,
                   std::placeholders::_2, *(T4 *)params->at(3).get());
}

template <class T0, class T1, class T2, class T3, class T4>
std::function<T0(T2 &, T4 &)>
binary_call_4_2_4(T0 (*func)(T1 &, T2 &, T3 &, T4 &),
                  const SharedArgumentsPtr &params) {
  return std::bind(func, *(T1 *)params->at(0).get(), std::placeholders::_1,
                   *(T3 *)params->at(2).get(), std::placeholders::_2);
}

template <class T0, class T1, class T2, class T3, class T4>
std::function<T0(T3 &, T4 &)>
binary_call_4_3_4(T0 (*func)(T1 &, T2 &, T3 &, T4 &),
                  const SharedArgumentsPtr &params) {
  return std::bind(func, *(T1 *)params->at(0).get(), *(T2 *)params->at(1).get(),
                   std::placeholders::_1, std::placeholders::_2);
}

template <class T0, class T1, class T2, class T3, class T4, class T5>
std::function<T0(T1 &, T2 &)>
binary_call_5_1_2(T0 (*func)(T1 &, T2 &, T3 &, T4 &, T5 &),
                  const SharedArgumentsPtr &params) {
  return std::bind(func, std::placeholders::_1, std::placeholders::_2,
                   *(T3 *)params->at(2).get(), *(T4 *)params->at(3).get(),
                   *(T5 *)params->at(4).get());
}

template <class T0, class T1, class T2, class T3, class T4, class T5>
std::function<T0(T1 &, T3 &)>
binary_call_5_1_3(T0 (*func)(T1 &, T2 &, T3 &, T4 &, T5 &),
                  const SharedArgumentsPtr &params) {
  return std::bind(func, std::placeholders::_1, *(T2 *)params->at(1).get(),
                   std::placeholders::_2, *(T4 *)params->at(3).get(),
                   *(T5 *)params->at(4).get());
}

template <class T0, class T1, class T2, class T3, class T4, class T5>
std::function<T0(T1 &, T4 &)>
binary_call_5_1_4(T0 (*func)(T1 &, T2 &, T3 &, T4 &, T5 &),
                  const SharedArgumentsPtr &params) {
  return std::bind(func, std::placeholders::_1, *(T2 *)params->at(1).get(),
                   *(T3 *)params->at(2).get(), std::placeholders::_2,
                   *(T5 *)params->at(4).get());
}

template <class T0, class T1, class T2, class T3, class T4, class T5>
std::function<T0(T1 &, T5 &)>
binary_call_5_1_5(T0 (*func)(T1 &, T2 &, T3 &, T4 &, T5 &),
                  const SharedArgumentsPtr &params) {
  return std::bind(func, std::placeholders::_1, *(T2 *)params->at(1).get(),
                   *(T3 *)params->at(2).get(), *(T4 *)params->at(3).get(),
                   std::placeholders::_2);
}

template <class T0, class T1, class T2, class T3, class T4, class T5>
std::function<T0(T2 &, T3 &)>
binary_call_5_2_3(T0 (*func)(T1 &, T2 &, T3 &, T4 &, T5 &),
                  const SharedArgumentsPtr &params) {
  return std::bind(func, *(T1 *)params->at(0).get(), std::placeholders::_1,
                   std::placeholders::_2, *(T4 *)params->at(3).get(),
                   *(T5 *)params->at(4).get());
}

template <class T0, class T1, class T2, class T3, class T4, class T5>
std::function<T0(T2 &, T4 &)>
binary_call_5_2_4(T0 (*func)(T1 &, T2 &, T3 &, T4 &, T5 &),
                  const SharedArgumentsPtr &params) {
  return std::bind(func, *(T1 *)params->at(0).get(), std::placeholders::_1,
                   *(T3 *)params->at(2).get(), std::placeholders::_2,
                   *(T5 *)params->at(4).get());
}

template <class T0, class T1, class T2, class T3, class T4, class T5>
std::function<T0(T2 &, T5 &)>
binary_call_5_2_5(T0 (*func)(T1 &, T2 &, T3 &, T4 &, T5 &),
                  const SharedArgumentsPtr &params) {
  return std::bind(func, *(T1 *)params->at(0).get(), std::placeholders::_1,
                   *(T3 *)params->at(2).get(), *(T4 *)params->at(3).get(),
                   std::placeholders::_2);
}

template <class T0, class T1, class T2, class T3, class T4, class T5>
std::function<T0(T3 &, T4 &)>
binary_call_5_3_4(T0 (*func)(T1 &, T2 &, T3 &, T4 &, T5 &),
                  const SharedArgumentsPtr &params) {
  return std::bind(func, *(T1 *)params->at(0).get(), *(T2 *)params->at(1).get(),
                   std::placeholders::_1, std::placeholders::_2,
                   *(T5 *)params->at(4).get());
}

template <class T0, class T1, class T2, class T3, class T4, class T5>
std::function<T0(T3 &, T5 &)>
binary_call_5_3_5(T0 (*func)(T1 &, T2 &, T3 &, T4 &, T5 &),
                  const SharedArgumentsPtr &params) {
  return std::bind(func, *(T1 *)params->at(0).get(), *(T2 *)params->at(1).get(),
                   std::placeholders::_1, *(T4 *)params->at(3).get(),
                   std::placeholders::_2);
}

template <class T0, class T1, class T2, class T3, class T4, class T5>
std::function<T0(T4 &, T5 &)>
binary_call_5_4_5(T0 (*func)(T1 &, T2 &, T3 &, T4 &, T5 &),
                  const SharedArgumentsPtr &params) {
  return std::bind(func, *(T1 *)params->at(0).get(), *(T2 *)params->at(1).get(),
                   *(T3 *)params->at(2).get(), std::placeholders::_1,
                   std::placeholders::_2);
}

template <typename T0, typename... ArgsType>
static std::shared_ptr<BinaryCaller>
get_binary_caller(T0 (*func)(ArgsType &...), const ConfigureOperator &opr) {
  if constexpr (sizeof...(ArgsType) > 5) {
    throw std::runtime_error("len(argvs) > 5");
  } else if constexpr (sizeof...(ArgsType) == 1) {
    throw std::runtime_error("len(argvs) == 0");
  } else if constexpr (sizeof...(ArgsType) == 2) {
    const FunctionInputType &type = opr.get_input_type();
    const SharedArgumentsPtr &params = opr.get_args();
    assert(params->size() == 2);
    assert(type == FunctionInputType::FI_Binary_S_2_L_1_2_Type);
    auto myfunc = binary_call_2_1_2(func, params);
    std::string arg1 = *(std::string *)params->at(0).get();
    std::string arg2 = *(std::string *)params->at(1).get();
    return std::make_shared<BinaryCaller>(get_binary_func(myfunc), arg1, arg2);
  } else if constexpr (sizeof...(ArgsType) == 3) {
    const FunctionInputType &type = opr.get_input_type();
    const SharedArgumentsPtr &params = opr.get_args();
    assert(params->size() == 3);
    if (type == FunctionInputType::FI_Binary_S_3_L_1_2_Type) {
      auto myfunc = binary_call_3_1_2(func, params);
      std::string arg1 = *(std::string *)params->at(0).get();
      std::string arg2 = *(std::string *)params->at(1).get();
      return std::make_shared<BinaryCaller>(get_binary_func(myfunc), arg1,
                                            arg2);
    } else if (type == FunctionInputType::FI_Binary_S_3_L_1_3_Type) {
      auto myfunc = binary_call_3_1_3(func, params);
      std::string arg1 = *(std::string *)params->at(0).get();
      std::string arg2 = *(std::string *)params->at(2).get();
      return std::make_shared<BinaryCaller>(get_binary_func(myfunc), arg1,
                                            arg2);
    } else if (type == FunctionInputType::FI_Binary_S_3_L_2_3_Type) {
      auto myfunc = binary_call_3_2_3(func, params);
      std::string arg1 = *(std::string *)params->at(1).get();
      std::string arg2 = *(std::string *)params->at(2).get();
      return std::make_shared<BinaryCaller>(get_binary_func(myfunc), arg1,
                                            arg2);
    }
  } else if constexpr (sizeof...(ArgsType) == 4) {
    const FunctionInputType &type = opr.get_input_type();
    const SharedArgumentsPtr &params = opr.get_args();
    assert(params->size() == 4);
    if (type == FunctionInputType::FI_Binary_S_4_L_1_2_Type) {
      auto myfunc = binary_call_4_1_2(func, params);
      std::string arg1 = *(std::string *)params->at(0).get();
      std::string arg2 = *(std::string *)params->at(1).get();
      return std::make_shared<BinaryCaller>(get_binary_func(myfunc), arg1,
                                            arg2);
    } else if (type == FunctionInputType::FI_Binary_S_4_L_1_3_Type) {
      auto myfunc = binary_call_4_1_3(func, params);
      std::string arg1 = *(std::string *)params->at(0).get();
      std::string arg2 = *(std::string *)params->at(2).get();
      return std::make_shared<BinaryCaller>(get_binary_func(myfunc), arg1,
                                            arg2);
    } else if (type == FunctionInputType::FI_Binary_S_4_L_1_4_Type) {
      auto myfunc = binary_call_4_1_4(func, params);
      std::string arg1 = *(std::string *)params->at(0).get();
      std::string arg2 = *(std::string *)params->at(3).get();
      return std::make_shared<BinaryCaller>(get_binary_func(myfunc), arg1,
                                            arg2);
    } else if (type == FunctionInputType::FI_Binary_S_4_L_2_3_Type) {
      auto myfunc = binary_call_4_2_3(func, params);
      std::string arg1 = *(std::string *)params->at(1).get();
      std::string arg2 = *(std::string *)params->at(2).get();
      return std::make_shared<BinaryCaller>(get_binary_func(myfunc), arg1,
                                            arg2);
    } else if (type == FunctionInputType::FI_Binary_S_4_L_2_4_Type) {
      auto myfunc = binary_call_4_2_4(func, params);
      std::string arg1 = *(std::string *)params->at(1).get();
      std::string arg2 = *(std::string *)params->at(3).get();
      return std::make_shared<BinaryCaller>(get_binary_func(myfunc), arg1,
                                            arg2);
    } else if (type == FunctionInputType::FI_Binary_S_4_L_3_4_Type) {
      auto myfunc = binary_call_4_3_4(func, params);
      std::string arg1 = *(std::string *)params->at(2).get();
      std::string arg2 = *(std::string *)params->at(3).get();
      return std::make_shared<BinaryCaller>(get_binary_func(myfunc), arg1,
                                            arg2);
    }
  } else if constexpr (sizeof...(ArgsType) == 5) {
    const FunctionInputType &type = opr.get_input_type();
    const SharedArgumentsPtr &params = opr.get_args();
    assert(params->size() == 5);
    if (type == FunctionInputType::FI_Binary_S_5_L_1_2_Type) {
      auto myfunc = binary_call_5_1_2(func, params);
      std::string arg1 = *(std::string *)params->at(0).get();
      std::string arg2 = *(std::string *)params->at(1).get();
      return std::make_shared<BinaryCaller>(get_binary_func(myfunc), arg1,
                                            arg2);
    } else if (type == FunctionInputType::FI_Binary_S_5_L_1_3_Type) {
      auto myfunc = binary_call_5_1_3(func, params);
      std::string arg1 = *(std::string *)params->at(0).get();
      std::string arg2 = *(std::string *)params->at(2).get();
      return std::make_shared<BinaryCaller>(get_binary_func(myfunc), arg1,
                                            arg2);
    } else if (type == FunctionInputType::FI_Binary_S_5_L_1_4_Type) {
      auto myfunc = binary_call_5_1_4(func, params);
      std::string arg1 = *(std::string *)params->at(0).get();
      std::string arg2 = *(std::string *)params->at(3).get();
      return std::make_shared<BinaryCaller>(get_binary_func(myfunc), arg1,
                                            arg2);
    } else if (type == FunctionInputType::FI_Binary_S_5_L_1_5_Type) {
      auto myfunc = binary_call_5_1_5(func, params);
      std::string arg1 = *(std::string *)params->at(0).get();
      std::string arg2 = *(std::string *)params->at(4).get();
      return std::make_shared<BinaryCaller>(get_binary_func(myfunc), arg1,
                                            arg2);
    } else if (type == FunctionInputType::FI_Binary_S_5_L_2_3_Type) {
      auto myfunc = binary_call_5_2_3(func, params);
      std::string arg1 = *(std::string *)params->at(1).get();
      std::string arg2 = *(std::string *)params->at(2).get();
      return std::make_shared<BinaryCaller>(get_binary_func(myfunc), arg1,
                                            arg2);
    } else if (type == FunctionInputType::FI_Binary_S_5_L_2_4_Type) {
      auto myfunc = binary_call_5_2_4(func, params);
      std::string arg1 = *(std::string *)params->at(1).get();
      std::string arg2 = *(std::string *)params->at(3).get();
      return std::make_shared<BinaryCaller>(get_binary_func(myfunc), arg1,
                                            arg2);
    } else if (type == FunctionInputType::FI_Binary_S_5_L_2_5_Type) {
      auto myfunc = binary_call_5_2_5(func, params);
      std::string arg1 = *(std::string *)params->at(1).get();
      std::string arg2 = *(std::string *)params->at(4).get();
      return std::make_shared<BinaryCaller>(get_binary_func(myfunc), arg1,
                                            arg2);
    } else if (type == FunctionInputType::FI_Binary_S_5_L_3_4_Type) {
      auto myfunc = binary_call_5_3_4(func, params);
      std::string arg1 = *(std::string *)params->at(2).get();
      std::string arg2 = *(std::string *)params->at(3).get();
      return std::make_shared<BinaryCaller>(get_binary_func(myfunc), arg1,
                                            arg2);
    } else if (type == FunctionInputType::FI_Binary_S_5_L_3_5_Type) {
      auto myfunc = binary_call_5_3_5(func, params);
      std::string arg1 = *(std::string *)params->at(2).get();
      std::string arg2 = *(std::string *)params->at(4).get();
      return std::make_shared<BinaryCaller>(get_binary_func(myfunc), arg1,
                                            arg2);
    } else if (type == FunctionInputType::FI_Binary_S_5_L_4_5_Type) {
      auto myfunc = binary_call_5_4_5(func, params);
      std::string arg1 = *(std::string *)params->at(3).get();
      std::string arg2 = *(std::string *)params->at(4).get();
      return std::make_shared<BinaryCaller>(get_binary_func(myfunc), arg1,
                                            arg2);
    }
  }
  return nullptr;
}

#endif // LUBAN_FEATURE_OPERATOR_BINARY_HANDLER_HPP