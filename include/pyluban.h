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
  int size_;
  friend PyToolKit;

 private:
  void init(int size);

 public:
  PyEntityArray();
  ~PyEntityArray();
  int size();
  Entity *get(int index);
};

class PyToolKit {
 private:
  ToolKit *toolkit;

 public:
  PyToolKit() = delete;
  PyToolKit(const PyToolKit &) = delete;
  PyToolKit(const PyToolKit &&) = delete;
  PyToolKit(std::string config_file);
  ~PyToolKit();
  void process(char *features, int len, PyEntityArray &entity);
  void process_file(std::string input_file, std::string output_file);
};

#endif  // LUBAN_PYLUBAN_H
