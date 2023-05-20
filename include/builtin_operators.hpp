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

#include <assert.h>
#include <string>
#include <time.h>
#include <vector>

template <class T> static T _min(T &a, T &b) { return a < b ? a : b; }
static int64_t _mini(int64_t &a, int64_t &b) { return _min<int64_t>(a, b); }
static std::string _mins(std::string &a, std::string &b) {
  return _min<std::string>(a, b);
}
static float _minf(float &a, float &b) { return _min<float>(a, b); }

template <class T> static T _max(T &a, T &b) { return a > b ? a : b; }
static int64_t _maxi(int64_t &a, int64_t &b) { return _max<int64_t>(a, b); }
static std::string _maxs(std::string &a, std::string &b) {
  return _max<std::string>(a, b);
}
static float _maxf(float &a, float &b) { return _max<float>(a, b); }

static float _addf(float &a, float &b) { return a + b; }
static int64_t _addi(int64_t &a, int64_t &b) { return a + b; }

static float _subf(float &a, float &b) { return a - b; }
static int64_t _subi(int64_t &a, int64_t &b) { return a - b; }

static float _mulf(float &a, float &b) { return a * b; }
static int64_t _muli(int64_t &a, int64_t &b) { return a * b; }

static float _divf(float &a, float &b) {
  assert(b != 0);
  return a / b;
}
static int64_t _divi(int64_t &a, int64_t &b) {
  assert(b != 0);
  return a / b;
}

static int64_t _modi(int64_t &a, int64_t &b) {
  assert(b != 0);
  return a % b;
}

static int64_t _powi(int64_t &a, int64_t &b) { return int64_t(pow(a, b)); }
static float _powf(float &a, float &b) { return powf(a, b); }

static int64_t _round(float &x) { return int64_t(roundf(x)); }

static int64_t _floor(float &x) { return int64_t(floorf(x)); }

static int64_t _ceil(float &x) { return int64_t(ceilf(x)); }

static float _log(float &x) { return logf(x); }

static float _exp(float &x) { return expf(x); }

static float _log10(float &x) { return log10(x); }

static float _log2(float &x) { return log2f(x); }

static float _sqrt(float &x) { return sqrtf(x); }

static float _absf(float &x) { return abs(x); }
static int64_t _absi(int64_t &x) { return abs(x); }

static float _sin(float &x) { return sinf(x); }

static float _asin(float &x) { return asinf(x); }

static float _sinh(float &x) { return sinhf(x); }

static float _asinh(float &x) { return asinhf(x); }

static float _cos(float &x) { return cosf(x); }

static float _acos(float &x) { return acosf(x); }

static float _cosh(float &x) { return coshf(x); }

static float _acosh(float &x) { return acoshf(x); }

static float _tan(float &x) { return tanf(x); }

static float _atan(float &x) { return atanf(x); }

static float _tanh(float &x) { return tanhf(x); }

static float _atanh(float &x) { return atanhf(x); }

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

static int64_t unix_timestamp(std::string &t, std::string &format) {
  std::tm timeinfo = {};
  strptime(t.c_str(), format.c_str(), &timeinfo);
  return mktime(&timeinfo);
}

static std::string date_add(std::string &s, int64_t &n) {
  std::string format = "%Y%m%d";
  int64_t ts = unix_timestamp(s, format) + 86400 * n;
  return from_unixtime(ts, format);
}

static std::string date_sub(std::string &s, int64_t &n) {
  std::string format = "%Y%m%d";
  int64_t ts = unix_timestamp(s, format) - 86400 * n;
  return from_unixtime(ts, format);
}

static int64_t date_diff(std::string &d1, std::string &d2) {
  std::string format = "%Y%m%d";
  int64_t t1 = unix_timestamp(d1, format);
  int64_t t2 = unix_timestamp(d2, format);
  return int64_t((t1 - t2) / 86400);
}

static std::string reverse(std::string &s) {
  std::string ret;
  ret.assign(s.rbegin(), s.rend());
  return ret;
}

static std::string upper(std::string &s) {
  std::string ret(s.size(), 0);
  for (size_t i = 0; i < s.size(); i++) {
    ret[i] = toupper(s[i]);
  }
  return ret;
}

static std::string lower(std::string &s) {
  std::string ret(s.size(), 0);
  for (size_t i = 0; i < s.size(); i++) {
    ret[i] = tolower(s[i]);
  }
  return ret;
}

static std::string substr(std::string &s, int64_t &start, int64_t &len) {
  return s.substr(start, len);
}

static std::string concat(std::string &a, std::string &b) {
  std::string ret("");
  ret.reserve(a.size() + b.size());
  ret.append(a);
  ret.append(b);
  return ret;
}

static std::string _to_stringf(float &v) { return std::to_string(v); }
static std::string _to_stringi(int64_t &v) { return std::to_string(v); }

static int64_t _to_integerf(float &v) { return int64_t(v); }
static int64_t _to_integers(std::string &v) { return std::atoll(v.c_str()); }

static float _to_floati(int64_t &v) { return float(v); }
static float _to_floats(std::string &v) { return std::atof(v.c_str()); }

static int64_t now() {
  auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::system_clock::now().time_since_epoch());
  return now.count();
}

static float min_max(float &v, float &min, float &max) {
  assert(min != max);
  return (v - min) / (max - min);
}

static float z_score(float &v, float &mean, float &stdv) {
  assert(stdv > 0.0);
  return (v - mean) / stdv;
}

static int64_t binarize(float &v, float &threshold) {
  return v < threshold ? 0.0 : 1.0;
}

static int64_t bucketize(float &v, std::vector<float> &boundaries) {
  auto start = boundaries.data();
  auto end = start + boundaries.size();
  return std::upper_bound(start, end, v) - start;
}

static float box_cox(float &v, float &lambda) {
  return lambda == 0.0 ? logf(v) : (powf(v, lambda) - 1.0f) / lambda;
}

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

static std::vector<int64_t> topki(std::vector<int64_t> &src, int64_t &k) {
  return topk<int64_t>(src, k);
}

static std::vector<float> topkf(std::vector<float> &src, int64_t &k) {
  return topk<float>(src, k);
}

#endif // LUBAN_FEATURE_BUILTIN_OPERATORS_HPP