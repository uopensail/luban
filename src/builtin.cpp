#include "builtin.h"

int64_t mmh3(const std::string &key) {
  char ret[16];
  MurmurHash3_x64_128(key.c_str(), key.size(), 0, ret);
  return ((int64_t *)ret)[0] & mask;
}

int64_t _mod(int64_t &a, int64_t &b) {
  assert(b != 0);
  return a % b;
}

float _pow(float &a, float &b) { return powf(a, b); }

int64_t _round(float &x) { return int64_t(roundf(x)); }

int64_t _floor(float &x) { return int64_t(floorf(x)); }

int64_t _ceil(float &x) { return int64_t(ceilf(x)); }

float _floorf(float &x) { return (floorf(x)); }

float _ceilf(float &x) { return (ceilf(x)); }

float _log(float &x) { return logf(x); }

float _exp(float &x) { return expf(x); }

float _log10(float &x) { return log10(x); }

float _log2(float &x) { return log2f(x); }

float _sqrt(float &x) {
  if (x <= 0) {
    return 0.0;
  }
  return sqrtf(x);
}

float _sin(float &x) { return sinf(x); }

float _asin(float &x) { return asinf(x); }

float _sinh(float &x) { return sinhf(x); }

float _asinh(float &x) { return asinhf(x); }

float _cos(float &x) { return cosf(x); }

float _acos(float &x) { return acosf(x); }

float _cosh(float &x) { return coshf(x); }

float _acosh(float &x) { return acoshf(x); }

float _tan(float &x) { return tanf(x); }

float _atan(float &x) { return atanf(x); }

float _tanh(float &x) { return tanhf(x); }

float _atanh(float &x) { return atanhf(x); }

float _sigmoid(float &x) { return 1.0 / (1.0 + expf(-x)); }

std::string year() {
  time_t t = time(nullptr);
  char tmp[64] = {0};
  strftime(tmp, sizeof(tmp), "%Y", localtime(&t));
  return std::string((char *)tmp);
}

std::string month() {
  time_t t = time(nullptr);
  char tmp[64] = {0};
  strftime(tmp, sizeof(tmp), "%m", localtime(&t));
  return std::string((char *)tmp);
}

std::string day() {
  time_t t = time(nullptr);
  char tmp[64] = {0};
  strftime(tmp, sizeof(tmp), "%d", localtime(&t));
  return std::string((char *)tmp);
}

std::string date() {
  time_t t = time(nullptr);
  char tmp[64] = {0};
  strftime(tmp, sizeof(tmp), "%Y%m%d", localtime(&t));
  return std::string((char *)tmp);
}

std::string hour() {
  time_t t = time(nullptr);
  char tmp[64] = {0};
  strftime(tmp, sizeof(tmp), "%H", localtime(&t));
  return std::string((char *)tmp);
}

std::string minute() {
  time_t t = time(nullptr);
  char tmp[64] = {0};
  strftime(tmp, sizeof(tmp), "%M", localtime(&t));
  return std::string((char *)tmp);
}

std::string second() {
  time_t t = time(nullptr);
  char tmp[64] = {0};
  strftime(tmp, sizeof(tmp), "%S", localtime(&t));
  return std::string((char *)tmp);
}

int64_t now() {
  auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::system_clock::now().time_since_epoch());
  return now.count();
}

std::string from_unixtime(int64_t &ts, std::string &format) {
  char tmp[64] = {0};
  time_t t = ts;
  strftime(tmp, sizeof(tmp), format.c_str(), localtime(&t));
  return std::string((char *)tmp);
}

int64_t unix_timestamp(std::string &t, std::string &format) {
  std::tm timeinfo = {};
  strptime(t.c_str(), format.c_str(), &timeinfo);
  return mktime(&timeinfo);
}

std::string date_add(std::string &s, int64_t &n) {
  std::string format = "%Y-%m-%d";
  int64_t ts = unix_timestamp(s, format) + 86400 * n;
  return from_unixtime(ts, format);
}

std::string date_sub(std::string &s, int64_t &n) {
  std::string format = "%Y-%m-%d";
  int64_t ts = unix_timestamp(s, format) - 86400 * n;
  return from_unixtime(ts, format);
}

int64_t date_diff(std::string &d1, std::string &d2) {
  std::string format = "%Y-%m-%d";
  int64_t t1 = unix_timestamp(d1, format);
  int64_t t2 = unix_timestamp(d2, format);
  return int64_t((t1 - t2) / 86400);
}

std::string reverse(std::string &s) {
  std::string ret;
  ret.assign(s.rbegin(), s.rend());
  return ret;
}

std::string upper(std::string &s) {
  std::string ret(s.size(), 0);
  for (size_t i = 0; i < s.size(); i++) {
    ret[i] = toupper(s[i]);
  }
  return ret;
}

std::string lower(std::string &s) {
  std::string ret(s.size(), 0);
  for (size_t i = 0; i < s.size(); i++) {
    ret[i] = tolower(s[i]);
  }
  return ret;
}

std::string substr(std::string &s, int64_t &start, int64_t &len) {
  return s.substr(start, len);
}

std::string concat(std::string &a, std::string &b) {
  std::string ret("");
  if (a.size() > 0) {
    ret.append(a);
  }
  if (b.size() > 0) {
    ret.append(b);
  }
  return ret;
}

float z_score(float &v, float &mean, float &stdv) {
  assert(stdv > 0.0);
  return (v - mean) / stdv;
}

float box_cox(float &v, float &lambda) {
  return lambda == 0.0 ? logf(v) : (powf(v, lambda) - 1.0f) / lambda;
}

std::vector<float> normalize(std::vector<float> &src, float &norm) {
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

std::vector<std::string> cross(std::vector<std::string> &srcA,
                               std::vector<std::string> &srcB) {
  std::vector<std::string> ret;
  for (auto &va : srcA) {
    for (auto &vb : srcB) {
      ret.push_back(concat(va, vb));
    }
  }
  return ret;
}

std::vector<std::string> concat_ws(std::string &a, std::vector<std::string> &srcB) {
  std::vector<std::string> ret;
  for (auto &vb : srcB) {
      ret.push_back(concat(a, vb));
  }
  return ret;
}