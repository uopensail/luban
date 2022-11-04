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
#include <functional>

#include "feature_helper.hpp"
#include "feature_operator_configure.hpp"
#include "feature_operator_runtime.hpp"
template <class T0, class T1>
std::function<T0(T1 &)> call(T0 (*func)(T1 &),
                             const SharedArgumentsPtr &params) {}

template <class T0, class T1, class T2>
std::function<T0(T1 &)> call(T0 (*func)(T1 &, T2 &),
                             const SharedArgumentsPtr &params) {}

template <class T0, class T1, class T2>
std::function<T0(T2 &)> call(T0 (*func)(T1 &, T2 &),
                             const SharedArgumentsPtr &params) {}

template <class T0, class T1, class T2, class T3>
T0 call(T0 (*func)(T1 &, T2 &, T3 &), const SharedArgumentsPtr &params) {}

template <class T0, class T1, class T2, class T3>
std::function<T0(T1 &)> call(T0 (*func)(T1 &, T2 &, T3 &),
                             const SharedArgumentsPtr &params) {}

template <class T0, class T1, class T2, class T3>
std::function<T0(T2 &)> call(T0 (*func)(T1 &, T2 &, T3 &),
                             const SharedArgumentsPtr &params) {}

template <class T0, class T1, class T2, class T3>
std::function<T0(T3 &)> call(T0 (*func)(T1 &, T2 &, T3 &),
                             const SharedArgumentsPtr &params) {}

template <class T0, class T1, class T2, class T3, class T4>
std::function<T0(T1 &)> call(T0 (*func)(T1 &, T2 &, T3 &, T4 &),
                             const SharedArgumentsPtr &params) {}

template <class T0, class T1, class T2, class T3, class T4>
std::function<T0(T2 &)> call(T0 (*func)(T1 &, T2 &, T3 &, T4 &),
                             const SharedArgumentsPtr &params) {}

template <class T0, class T1, class T2, class T3, class T4>
std::function<T0(T3 &)> call(T0 (*func)(T1 &, T2 &, T3 &, T4 &),
                             const SharedArgumentsPtr &params) {}

template <class T0, class T1, class T2, class T3, class T4>
std::function<T0(T4 &)> call(T0 (*func)(T1 &, T2 &, T3 &, T4 &),
                             const SharedArgumentsPtr &params) {}

template <class T0, class T1, class T2, class T3, class T4, class T5>
std::function<T0(T1 &)> call(T0 (*func)(T1 &, T2 &, T3 &, T4 &, T5 &),
                             const SharedArgumentsPtr &params) {}

template <class T0, class T1, class T2, class T3, class T4, class T5>
std::function<T0(T2 &)> call(T0 (*func)(T1 &, T2 &, T3 &, T4 &, T5 &),
                             const SharedArgumentsPtr &params) {}

template <class T0, class T1, class T2, class T3, class T4, class T5>
std::function<T0(T3 &)> call(T0 (*func)(T1 &, T2 &, T3 &, T4 &, T5 &),
                             const SharedArgumentsPtr &params) {}

template <class T0, class T1, class T2, class T3, class T4, class T5>
std::function<T0(T4 &)> call(T0 (*func)(T1 &, T2 &, T3 &, T4 &, T5 &),
                             const SharedArgumentsPtr &params) {}

template <class T0, class T1, class T2, class T3, class T4, class T5>
std::function<T0(T5 &)> call(T0 (*func)(T1 &, T2 &, T3 &, T4 &, T5 &),
                             const SharedArgumentsPtr &params) {}

#endif  // LUBAN_FEATURE_OPERATOR_HANDLER_HPP