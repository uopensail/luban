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

#include <string>
#include <vector>

#include "MurmurHash3.h"

int64_t mmh3(const std::string &key);

template <typename T>
T _add(T &a, T &b) {
  return a + b;
}

template <typename T>
T _sub(T &a, T &b) {
  return a - b;
}

template <typename T>
T _mul(T &a, T &b) {
  return a * b;
}

template <typename T>
T _div(T &a, T &b) {
  assert(b != 0);
  return a / b;
}

int64_t _mod(int64_t &a, int64_t &b);
float _pow(float &a, float &b);
int64_t _round(float &x);
int64_t _floor(float &x);
int64_t _ceil(float &x);
float _log(float &x);
float _exp(float &x);
float _log10(float &x);
float _log2(float &x);
float _sqrt(float &x);
float _abs(float &x);
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
float min(std::vector<float> &src);
float max(std::vector<float> &src);
float average(std::vector<float> &src);
float variance(std::vector<float> &src);
float stddev(std::vector<float> &src);
float norm(std::vector<float> &src, float &n);
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
float min_max(float &v, float &min, float &max);
float z_score(float &v, float &mean, float &stdv);
int64_t binarize(float &v, float &threshold);
int64_t bucketize(float &v, std::vector<float> &boundaries);
float box_cox(float &v, float &lambda);
std::vector<float> normalize(std::vector<float> &src, float &norm);

template <typename T>
std::vector<T> topk(std::vector<T> &src, int64_t &k) {
  std::vector<T> dst;
  int64_t len = std::min<int64_t>(k, src.size());
  dst.assign(src.begin(), src.begin() + len);
  return dst;
}

std::vector<std::string> cross(std::vector<std::string> &srcA,
                               std::vector<std::string> &srcB);

#endif  // LUBAN_BUILTIN_H