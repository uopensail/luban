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

#ifndef LUBAN_HELPER_H
#define LUBAN_HELPER_H

#pragma once

#include <functional>
#include <iostream>
#include <typeinfo>
#include <vector>

namespace luban {

template <typename T>
constexpr std::string_view get_template_type() {
  std::string_view name, prefix, suffix;
#ifdef __clang__
  name = __PRETTY_FUNCTION__;
  prefix = "auto type_name() [T = ";
  suffix = "]";
#elif defined(__GNUC__)
  name = __PRETTY_FUNCTION__;
  prefix = "constexpr auto type_name() [with T = ";
  suffix = "]";
#elif defined(_MSC_VER)
  name = __FUNCSIG__;
  prefix = "auto __cdecl type_name<";
  suffix = ">(void)";
#endif
  name.remove_prefix(prefix.size());
  name.remove_suffix(suffix.size());
  return name;
}

#define print_type(T) std::cout << get_template_type<T>() << std::endl;

template <typename T>
void print_template_type() {
  print_type(T);
}

template <typename T1, typename T2, typename... Ts>
void print_template_type() {
  if (sizeof...(Ts) == 0) {
    print_type(T1);
    print_type(T2);
  } else {
    print_type(T1);
    print_template_type<T2, Ts...>();
  }
}

}  // namespace luban
#endif  // LUBAN_HELPER_H