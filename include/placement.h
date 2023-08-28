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

#ifndef LUBAN_PLACEMENT_H
#define LUBAN_PLACEMENT_H

#pragma once

#include "common.h"
#include "features.h"

namespace luban {

typedef struct GroupConfigure {
  int id;
  int64_t width;
  DataType type;
} GroupConfigure;

typedef struct FeatureConfigure {
  bool hash;
  DataType type;
  int group;
  int dim;
  int64_t offset;
  Padding padding;
} FeatureConfigure;

class Row {
 public:
  Row() = delete;
  Row(DataType type, int64_t cols);
  ~Row();
  void put(SharedParameter value, const FeatureConfigure &conf);

 public:
  DataType m_type;
  int64_t m_cols;
  int64_t m_size;
  char *m_data;
};

class Matrix {
 public:
  Matrix() = delete;
  Matrix(DataType type, int64_t rows, int64_t cols);
  ~Matrix();
  void put(int64_t row, SharedParameter value, const FeatureConfigure &conf);

 public:
  DataType m_type;
  int64_t m_rows;
  int64_t m_cols;
  int64_t m_size;
  char *m_data;
};

class Matrices {
 public:
  Matrices() = default;
  ~Matrices() = default;
  std::shared_ptr<Matrix> operator[](int index);

 public:
  std::vector<std::shared_ptr<Matrix>> m_matrices;
};

class Rows {
 public:
  Rows() = default;
  ~Rows() = default;
  std::shared_ptr<Row> operator[](int index);

 public:
  std::vector<std::shared_ptr<Row>> m_rows;
};

class Placement {
 public:
  Placement() = delete;
  explicit Placement(std::string_view config);
  explicit Placement(const json &value);
  ~Placement() = default;
  std::shared_ptr<Matrices> matrices(int64_t batch_size);
  std::shared_ptr<Rows> rows();
  void call(Features &features, std::shared_ptr<Matrices> &m, int64_t row);
  void call(Features &features, std::shared_ptr<Rows> &r);

 private:
  void parse(const json &doc);

 public:
  std::vector<GroupConfigure> m_groups;
  std::unordered_map<std::string, FeatureConfigure> m_features;
};

}  // namespace luban

#endif  // LUBAN_PLACEMENT_H