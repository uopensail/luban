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

#ifndef LUBAN_FEATURE_BUILTIN_OPERATORS_HPP
#define LUBAN_FEATURE_BUILTIN_OPERATORS_HPP

#pragma once

#include <math.h>

#include <string>
#include <vector>

#include "feature_operator_handler.hpp"

// add
static float _add(float &a, float &b) { return a + b; }
unary_function_wrapper(_add);
binary_function_wrapper(_add);

// sub
static float _sub(float &a, float &b) { return a - b; }
unary_function_wrapper(_sub);
binary_function_wrapper(_sub);

// mul
static float _mul(float &a, float &b) { return a * b; }
unary_function_wrapper(_mul);
binary_function_wrapper(_mul);

// div
static float _div(float &a, float &b) {
  assert(b != 0.0);
  return a / b;
}
unary_function_wrapper(_div);
binary_function_wrapper(_div);

// mod
static int64_t _mod(int64_t &a, int64_t &b) {
  assert(b != 0);
  return a % b;
}
unary_function_wrapper(_mod);
binary_function_wrapper(_mod);

// pow
static float _pow(float &a, float &b) { return powf(a, b); }
unary_function_wrapper(_pow);
binary_function_wrapper(_pow);

static float _floor(float &v) { return floorf(v); }
unary_function_wrapper(_floor);

static float _ceil(float &v) { return ceilf(v); }
unary_function_wrapper(_ceil);

static float _log(float &v) { return logf(v); }
unary_function_wrapper(_log);

static float _exp(float &v) { return expf(v); }
unary_function_wrapper(_exp);

static int64_t timestamp() {
  auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::system_clock::now().time_since_epoch());
  return now.count();
}
static std::string date() {
  time_t t = time(0);
  char tmp[9] = {0};
  strftime(tmp, sizeof(tmp), "%Y%m%d", localtime(&t));
  return std::string((char *)tmp);
}

static float min_max(float &v, float &min, float &max) {
  assert(min != max);
  return (v - min) / (max - min);
}
unary_function_wrapper(min_max);

static float z_score(float &v, float &mean, float &stdv) {
  assert(stdv != 0.0);
  return (v - mean) / stdv;
}
unary_function_wrapper(z_score);

static int64_t binarize(float &v, float &threshold) {
  return v < threshold ? 0.0 : 1.0;
}
unary_function_wrapper(binarize);

//分桶
static int64_t bucketize(float &v, std::vector<float> &boundaries) {
  auto start = boundaries.data();
  auto end = start + boundaries.size();
  return std::upper_bound(start, end, v) - start;
}

unary_function_wrapper(bucketize);

static float box_cox(float &v, float &lambda) {
  return lambda == 0.0 ? logf(v) : (powf(v, lambda) - 1.0f) / lambda;
}
unary_function_wrapper(box_cox);

static std::string substr(std::string &str, size_t &pos, size_t &len) {
  return str.substr(pos, len);
}
unary_function_wrapper(substr);

static std::string concat(std::string &a, std::string &b) {
  std::string v;
  v.reserve(a.size() + b.size());
  v += a;
  v += b;
  return v;
}
unary_function_wrapper(concat);
binary_function_wrapper(concat);

static std::vector<float> *normalize(std::vector<float> &src, float &norm) {
  assert(norm >= 1);
  std::vector<float> *dst = new std::vector<float>();
  dst->reserve(src.size());
  float norm_value = 0.0;
  for (auto &v : src) {
    norm_value += powf(abs(v), norm);
  }
  norm_value = powf(norm_value, 1.0 / norm);

  for (auto &v : src) {
    dst->push_back(v / norm_value);
  }
  return dst;
}
unary_function_wrapper(normalize);

static std::vector<std::string> *topks(std::vector<std::string> &src,
                                       size_t &k) {
  std::vector<std::string> *dst = new std::vector<std::string>();
  dst->reserve(k);
  size_t count = 0;
  for (auto &v : src) {
    if (count < k) {
      dst->push_back(v);
      count++;
    }
  }
  return dst;
}
unary_function_wrapper(topks);

static std::vector<int64_t> *topki(std::vector<int64_t> &src, size_t &k) {
  std::vector<int64_t> *dst = new std::vector<int64_t>();
  dst->reserve(k);
  size_t count = 0;
  for (auto &v : src) {
    if (count < k) {
      dst->push_back(v);
      count++;
    }
  }
  return dst;
}
unary_function_wrapper(topki);

static std::vector<float> *topkf(std::vector<float> &src, size_t &k) {
  std::vector<float> *dst = new std::vector<float>();
  dst->reserve(k);
  size_t count = 0;
  for (auto &v : src) {
    if (count < k) {
      dst->push_back(v);
      count++;
    }
  }
  return dst;
}
unary_function_wrapper(topkf);

#endif  // LUBAN_FEATURE_BUILTIN_OPERATORS_HPP