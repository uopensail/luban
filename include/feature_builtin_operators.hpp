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

#ifndef LUBAN_FEATURE_BUILTIN_OPERATORS_HPP
#define LUBAN_FEATURE_BUILTIN_OPERATORS_HPP

#pragma once

#include <math.h>

#include <string>
#include <vector>

#include "feature_operator_handler.hpp"

template <class T>
static T _min(T &a, T &b) {
  return a < b ? a : b;
}
static int64_t _mini(int64_t &a, int64_t &b) { return _min<int64_t>(a, b); }
static std::string _mins(std::string &a, std::string &b) {
  return _min<std::string>(a, b);
}
static float _minf(float &a, float &b) { return _min<float>(a, b); }
unary_function_wrapper(_mini);
unary_function_wrapper(_mins);
unary_function_wrapper(_minf);
binary_function_wrapper(_mini);
binary_function_wrapper(_mins);
binary_function_wrapper(_minf);

template <class T>
static T _max(T &a, T &b) {
  return a > b ? a : b;
}
static int64_t _maxi(int64_t &a, int64_t &b) { return _max<int64_t>(a, b); }
static std::string _maxs(std::string &a, std::string &b) {
  return _max<std::string>(a, b);
}
static float _maxf(float &a, float &b) { return _max<float>(a, b); }
unary_function_wrapper(_maxi);
unary_function_wrapper(_maxs);
unary_function_wrapper(_maxf);
binary_function_wrapper(_maxi);
binary_function_wrapper(_maxs);
binary_function_wrapper(_maxf);

static float _addf(float &a, float &b) { return a + b; }
static int64_t _addi(int64_t &a, int64_t &b) { return a + b; }
unary_function_wrapper(_addf);
unary_function_wrapper(_addi);
binary_function_wrapper(_addf);
binary_function_wrapper(_addi);

static float _subf(float &a, float &b) { return a - b; }
static int64_t _subi(int64_t &a, int64_t &b) { return a - b; }
unary_function_wrapper(_subf);
unary_function_wrapper(_subi);
binary_function_wrapper(_subf);
binary_function_wrapper(_subi);

static float _mulf(float &a, float &b) { return a * b; }
static int64_t _muli(int64_t &a, int64_t &b) { return a * b; }
unary_function_wrapper(_mulf);
unary_function_wrapper(_muli);
binary_function_wrapper(_mulf);
binary_function_wrapper(_muli);

static float _divf(float &a, float &b) {
  assert(b != 0);
  return a / b;
}
static int64_t _divi(int64_t &a, int64_t &b) {
  assert(b != 0);
  return a / b;
}
unary_function_wrapper(_divf);
unary_function_wrapper(_divi);
binary_function_wrapper(_divf);
binary_function_wrapper(_divi);

static int64_t _modi(int64_t &a, int64_t &b) {
  assert(b != 0);
  return a % b;
}
unary_function_wrapper(_modi);
binary_function_wrapper(_modi);

static int64_t _powi(int64_t &a, int64_t &b) { return int64_t(pow(a, b)); }
static float _powf(float &a, float &b) { return powf(a, b); }
unary_function_wrapper(_powi);
unary_function_wrapper(_powf);
binary_function_wrapper(_powi);
binary_function_wrapper(_powf);

static int64_t _round(float &x) { return int64_t(roundf(x)); }
unary_function_wrapper(_round);

static int64_t _floor(float &x) { return int64_t(floorf(x)); }
unary_function_wrapper(_floor);

static int64_t _ceil(float &x) { return int64_t(ceilf(x)); }
unary_function_wrapper(_ceil);

static float _log(float &x) { return logf(x); }
unary_function_wrapper(_log);

static float _exp(float &x) { return expf(x); }
unary_function_wrapper(_exp);

static float _log10(float &x) { return log10(x); }
unary_function_wrapper(_log10);

static float _log2(float &x) { return log2f(x); }
unary_function_wrapper(_log2);

static float _sqrt(float &x) { return sqrtf(x); }
unary_function_wrapper(_sqrt);

static float _absf(float &x) { return abs(x); }
static int64_t _absi(int64_t &x) { return abs(x); }
unary_function_wrapper(_absf);
unary_function_wrapper(_absi);

static float _sin(float &x) { return sinf(x); }
unary_function_wrapper(_sin);

static float _asin(float &x) { return asinf(x); }
unary_function_wrapper(_asin);

static float _sinh(float &x) { return sinhf(x); }
unary_function_wrapper(_sinh);

static float _asinh(float &x) { return asinhf(x); }
unary_function_wrapper(_asinh);

static float _cos(float &x) { return cosf(x); }
unary_function_wrapper(_cos);

static float _acos(float &x) { return acosf(x); }
unary_function_wrapper(_acos);

static float _cosh(float &x) { return coshf(x); }
unary_function_wrapper(_cosh);

static float _acosh(float &x) { return acoshf(x); }
unary_function_wrapper(_acosh);

static float _tan(float &x) { return tanf(x); }
unary_function_wrapper(_tan);

static float _atan(float &x) { return atanf(x); }
unary_function_wrapper(_atan);

static float _tanh(float &x) { return tanhf(x); }
unary_function_wrapper(_tanh);

static float _atanh(float &x) { return atanhf(x); }
unary_function_wrapper(_atanh);

static std::string year() {
  time_t t = time(nullptr);
  char tmp[64] = {0};
  strftime(tmp, sizeof(tmp), "%Y", localtime(&t));
  return std::string((char *)tmp);
}

static std::string month() {
  time_t t = time(nullptr);
  char tmp[64] = {0};
  strftime(tmp, sizeof(tmp), "%m", localtime(&t));
  return std::string((char *)tmp);
}

static std::string day() {
  time_t t = time(nullptr);
  char tmp[64] = {0};
  strftime(tmp, sizeof(tmp), "%d", localtime(&t));
  return std::string((char *)tmp);
}

static std::string date() {
  time_t t = time(nullptr);
  char tmp[64] = {0};
  strftime(tmp, sizeof(tmp), "%Y%m%d", localtime(&t));
  return std::string((char *)tmp);
}

static std::string hour() {
  time_t t = time(nullptr);
  char tmp[64] = {0};
  strftime(tmp, sizeof(tmp), "%H", localtime(&t));
  return std::string((char *)tmp);
}

static std::string minute() {
  time_t t = time(nullptr);
  char tmp[64] = {0};
  strftime(tmp, sizeof(tmp), "%M", localtime(&t));
  return std::string((char *)tmp);
}

static std::string second() {
  time_t t = time(nullptr);
  char tmp[64] = {0};
  strftime(tmp, sizeof(tmp), "%S", localtime(&t));
  return std::string((char *)tmp);
}

static std::string from_unixtime(int64_t &ts, std::string &format) {
  char tmp[64] = {0};
  time_t t = ts;
  strftime(tmp, sizeof(tmp), format.c_str(), localtime(&t));
  return std::string((char *)tmp);
}
unary_function_wrapper(from_unixtime);

static int64_t unix_timestamp(std::string &t, std::string &format) {
  std::tm timeinfo = {};
  strptime(t.c_str(), format.c_str(), &timeinfo);
  return mktime(&timeinfo);
}

unary_function_wrapper(unix_timestamp);

static std::string date_add(std::string &s, int64_t &n) {
  std::string format = "%Y%m%d";
  int64_t ts = unix_timestamp(s, format) + 86400 * n;
  return from_unixtime(ts, format);
}
unary_function_wrapper(date_add);

static std::string date_sub(std::string &s, int64_t &n) {
  std::string format = "%Y%m%d";
  int64_t ts = unix_timestamp(s, format) - 86400 * n;
  return from_unixtime(ts, format);
}
unary_function_wrapper(date_sub);

static int64_t date_diff(std::string &d1, std::string &d2) {
  std::string format = "%Y%m%d";
  int64_t t1 = unix_timestamp(d1, format);
  int64_t t2 = unix_timestamp(d2, format);
  return int64_t((t1 - t2) / 86400);
}
unary_function_wrapper(date_diff);
binary_function_wrapper(date_diff);

static std::string reverse(std::string &s) {
  std::string ret;
  ret.assign(s.rbegin(), s.rend());
  reverse(s.begin(), s.end());
  return ret;
}
unary_function_wrapper(reverse);

static std::string upper(std::string &s) {
  std::string ret(s.size(), 0);
  for (size_t i = 0; i < s.size(); i++) {
    ret[i] = toupper(s[i]);
  }
  return ret;
}
unary_function_wrapper(upper);

static std::string lower(std::string &s) {
  std::string ret(s.size(), 0);
  for (size_t i = 0; i < s.size(); i++) {
    ret[i] = tolower(s[i]);
  }
  return ret;
}
unary_function_wrapper(lower);

static std::string substr(std::string &s, int64_t &start, int64_t &len) {
  return s.substr(start, len);
}
unary_function_wrapper(substr);

static std::string concat(std::string &a, std::string &b) {
  std::string ret("");
  ret.reserve(a.size() + b.size());
  ret.append(a);
  ret.append(b);
  return ret;
}
unary_function_wrapper(concat);
binary_function_wrapper(concat);

static std::string _to_stringf(float &v) { return std::to_string(v); }
static std::string _to_stringi(int64_t &v) { return std::to_string(v); }
unary_function_wrapper(_to_stringf);
unary_function_wrapper(_to_stringi);

static int64_t _to_integerf(float &v) { return int64_t(v); }
static int64_t _to_integers(std::string &v) { return std::atoll(v.c_str()); }
unary_function_wrapper(_to_integerf);
unary_function_wrapper(_to_stringi);

static float _to_floati(int64_t &v) { return float(v); }
static float _to_floats(std::string &v) { return std::atof(v.c_str()); }
unary_function_wrapper(_to_floati);
unary_function_wrapper(_to_floats);

static int64_t now() {
  auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::system_clock::now().time_since_epoch());
  return now.count();
}

static float min_max(float &v, float &min, float &max) {
  assert(min != max);
  return (v - min) / (max - min);
}
unary_function_wrapper(min_max);

static float z_score(float &v, float &mean, float &stdv) {
  assert(stdv > 0.0);
  return (v - mean) / stdv;
}
unary_function_wrapper(z_score);

static int64_t binarize(float &v, float &threshold) {
  return v < threshold ? 0.0 : 1.0;
}
unary_function_wrapper(binarize);

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

static std::vector<float> normalize(std::vector<float> &src, float &norm) {
  assert(norm >= 1);
  std::vector<float> dst;
  dst.reserve(src.size());
  float norm_value = 0.0;
  for (auto &v : src) {
    norm_value += powf(abs(v), norm);
  }
  norm_value = powf(norm_value, 1.0 / norm);

  for (auto &v : src) {
    dst.push_back(v / norm_value);
  }
  return dst;
}
unary_function_wrapper(normalize);

template <typename T>
static std::vector<T> topk(std::vector<T> &src, int64_t &k) {
  std::vector<T> dst;
  dst.reserve(k);
  size_t count = 0;
  for (auto &v : src) {
    if (count < k) {
      dst.push_back(v);
      count++;
    }
  }
  return dst;
}

static std::vector<std::string> topks(std::vector<std::string> &src,
                                      int64_t &k) {
  return topk<std::string>(src, k);
}
unary_function_wrapper(topks);

static std::vector<int64_t> topki(std::vector<int64_t> &src, int64_t &k) {
  return topk<int64_t>(src, k);
}
unary_function_wrapper(topki);

static std::vector<float> topkf(std::vector<float> &src, int64_t &k) {
  return topk<float>(src, k);
}
unary_function_wrapper(topkf);

#endif  // LUBAN_FEATURE_BUILTIN_OPERATORS_HPP