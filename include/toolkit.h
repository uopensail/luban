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

#ifndef LUBAN_TOOLKIT_H
#define LUBAN_TOOLKIT_H

#pragma once
#include <fstream>
#include <iostream>
#include <memory>

#include "operator.h"
#include "placement.h"

namespace luban {

class Toolkit {
public:
  Toolkit() = delete;
  Toolkit(const Toolkit &) = delete;
  Toolkit(const Toolkit &&) = delete;
  Toolkit(const std::string &config_file);
  ~Toolkit() = default;
  std::shared_ptr<Rows> process_item(SharedFeaturesPtr features);
  std::shared_ptr<Rows> process_user(SharedFeaturesPtr features);
  std::shared_ptr<Rows> process(SharedFeaturesPtr features);

public:
  std::shared_ptr<Operator> m_opr;
  std::shared_ptr<Placement> m_item_placer;
  std::shared_ptr<Placement> m_user_placer;
  std::vector<FunctionConfigure> m_item_funcs;
  std::vector<FunctionConfigure> m_user_funcs;
  std::vector<GroupConfigure> m_groups;
};

} // namespace luban
#endif // LUBAN_TOOLKIT_H