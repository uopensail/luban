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

#ifndef PYLUBAN_H
#define PYLUBAN_H

#pragma once

#include <string>
#include <vector>

#include "toolkit.hpp"

class PyToolKit;

class PyEntityArray {
private:
  EntityArray *data_;
  friend PyToolKit;

public:
  PyEntityArray();
  ~PyEntityArray();
  int size();
  Entity *get(int index);
};

class PyToolKit {
private:
  Toolkit *toolkit;

public:
  PyToolKit() = delete;
  PyToolKit(const PyToolKit &) = delete;
  PyToolKit(const PyToolKit &&) = delete;
  PyToolKit(std::string config_file);
  ~PyToolKit();
  void process(char *features, int len, PyEntityArray &entity);
  void process_file(std::string input_file, std::string output_file);
};

#endif // LUBAN_PYLUBAN_H
