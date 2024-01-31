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

#ifndef LUBAN_BUILTIN_H
#define LUBAN_BUILTIN_H

#pragma once

#include <assert.h>
#include <math.h>
#include <time.h>

#include "MurmurHash3.h"

#ifdef __linux__
#include <bits/stl_algo.h>
#endif

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

const int64_t mask = 0x0fffffffffffffffll;

int64_t mmh3(const std::string &key);

template <typename T> T _add(T &a, T &b) { return a + b; }

template <typename T> T _sub(T &a, T &b) { return a - b; }

template <typename T> T _mul(T &a, T &b) { return a * b; }

template <typename T> T _div(T &a, T &b) {
  assert(b != 0);
  return a / b;
}

int64_t _mod(int64_t &a, int64_t &b);
float _pow(float &a, float &b);
int64_t _round(float &x);
int64_t _floor(float &x);
int64_t _ceil(float &x);
float _floorf(float &x);
float _ceilf(float &x);
float _log(float &x);
float _exp(float &x);
float _log10(float &x);
float _log2(float &x);
float _sqrt(float &x);

template <typename T> T _abs(T &x) { return x < 0 ? -x : x; }

float _sin(float &x);
float _asin(float &x);
float _sinh(float &x);
float _asinh(float &x);
float _cos(float &x);
float _acos(float &x);
float _cosh(float &x);
float _acosh(float &x);
float _tan(float &x);
float _atan(float &x);
float _tanh(float &x);
float _atanh(float &x);
float _sigmoid(float &x);

template <typename T> T min(std::vector<float> &src) {
  assert(src.size() > 0);
  T ret = src[0];
  for (auto &v : src) {
    if (v < ret) {
      ret = v;
    }
  }
  return ret;
}

template <typename T> T max(std::vector<T> &src) {
  assert(src.size() > 0);
  T ret = src[0];
  for (auto &v : src) {
    if (v > ret) {
      ret = v;
    }
  }
  return ret;
}

template <typename T> float average(std::vector<T> &src) {
  assert(src.size() > 0);
  T ret = 0;
  float n = float(src.size());
  for (auto &v : src) {
    ret += v;
  }
  return static_cast<float>(ret) / n;
}

template <typename T> float variance(std::vector<T> &src) {
  if (src.size() <= 1) {
    return 0.0;
  }
  T sum_of_value = 0;
  T sum_of_square = 0;
  float n = float(src.size());
  for (auto &v : src) {
    sum_of_value += v;
    sum_of_square += v * v;
  }

  return (static_cast<float>(sum_of_square) -
          static_cast<float>(sum_of_value * sum_of_value) / n) /
         n;
}

template <typename T> float stddev(std::vector<T> &src) {
  float var = variance<T>(src);
  if (var <= 0) {
    return 0;
  }
  return sqrtf(var);
}

template <typename T> float norm(std::vector<T> &src, float &n) {
  float ret = 0.0;
  for (auto &v : src) {
    ret += powf(static_cast<float>(v), n);
  }
  return powf(ret, 1.0 / n);
}

std::string year();
std::string month();
std::string day();
std::string date();
std::string hour();
std::string minute();
std::string second();
int64_t now();
std::string from_unixtime(int64_t &ts, std::string &format);
int64_t unix_timestamp(std::string &t, std::string &format);
std::string date_add(std::string &s, int64_t &n);
std::string date_sub(std::string &s, int64_t &n);
int64_t date_diff(std::string &d1, std::string &d2);
std::string reverse(std::string &s);
std::string upper(std::string &s);
std::string lower(std::string &s);
std::string substr(std::string &s, int64_t &start, int64_t &len);
std::string concat(std::string &a, std::string &b);

template <typename T> float min_max(T &v, T &min, T &max) {
  assert(min != max);
  return static_cast<float>(v - min) / static_cast<float>(max - min);
}

float z_score(float &v, float &mean, float &stdv);

template <typename T> int64_t binarize(T &v, T &threshold) {
  return v < threshold ? 0 : 1;
}

template <typename T> int64_t bucketize(T &v, std::vector<T> &boundaries) {
  auto start = boundaries.data();
  auto end = start + boundaries.size();
  return std::upper_bound(start, end, v) - start;
}

float box_cox(float &v, float &lambda);

template <typename T>
std::vector<float> normalize(std::vector<T> &src, float &norm) {
  assert(norm >= 1);
  std::vector<float> dst;
  dst.reserve(src.size());
  float norm_value = 0.0;
  for (auto &v : src) {
    norm_value += powf(abs(static_cast<float>(v)), norm);
  }
  norm_value = powf(norm_value, 1.0 / norm);

  for (auto &v : src) {
    dst.push_back(v / norm_value);
  }
  return dst;
}

template <typename T> std::vector<T> topk(std::vector<T> &src, int64_t &k) {
  std::vector<T> dst;
  int64_t len = std::min<int64_t>(k, src.size());
  dst.assign(src.begin(), src.begin() + len);
  return dst;
}

template <typename T0, typename T1> T0 cast(T1 &value) {
  if constexpr (std::is_same_v<T0, int64_t>) {
    if constexpr (std::is_same_v<T1, int64_t>) {
      return value;
    } else if constexpr (std::is_same_v<T1, float>) {
      return static_cast<int64_t>(value);
    } else if constexpr (std::is_same_v<T1, std::string>) {
      return static_cast<int64_t>(std::stoll(value));
    }
    throw std::runtime_error("not implemented");
  } else if constexpr (std::is_same_v<T0, float>) {
    if constexpr (std::is_same_v<T1, int64_t>) {
      return static_cast<float>(value);
    } else if constexpr (std::is_same_v<T1, float>) {
      return value;
    } else if constexpr (std::is_same_v<T1, std::string>) {
      return std::stof(value);
    }
    throw std::runtime_error("type not supported");
  } else if constexpr (std::is_same_v<T0, std::string>) {
    if constexpr (std::is_same_v<T1, int64_t>) {
      return std::to_string(value);
    } else if constexpr (std::is_same_v<T1, float>) {
      return std::to_string(value);
    } else if constexpr (std::is_same_v<T1, std::string>) {
      return value;
    }
    throw std::runtime_error("type not supported");
  }
}

std::vector<std::string> cross(std::vector<std::string> &srcA,
                               std::vector<std::string> &srcB);
std::vector<std::string> concat_ws(std::string &a, std::vector<std::string> &srcB);
#endif // LUBAN_BUILTIN_H