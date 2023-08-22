#include "features.h"

namespace luban {

Features::Features(std::string_view value) {
  extract_features(value, m_features);
}

Features::Features(const std::string &value) {
  extract_features(value, m_features);
}

Features::Features(const std::initializer_list<std::string_view> &values) {
  for (auto &v : values) {
    extract_features(v, m_features);
  }
}

Features::Features(const std::initializer_list<std::string> &values) {
  for (auto &v : values) {
    extract_features(v, m_features);
  }
}

Features::Features(const std::vector<std::string_view> &values) {
  for (auto &v : values) {
    extract_features(v, m_features);
  }
}

Features::Features(const std::vector<std::string> &values) {
  for (auto &v : values) {
    extract_features(v, m_features);
  }
}

Features &Features::operator=(const Features &features) {
  if (this == &features) {
    return *this;
  }
  for (auto &kv : features.m_features) {
    m_features[kv.first] = kv.second;
  }
  return *this;
}

SharedParameter Features::operator[](const std::string &key) {
  auto it = m_features.find(key);
  if (it != m_features.end()) {
    return it->second;
  }
  return nullptr;
}

void Features::insert(const std::string &key, SharedParameter feature) {
  m_features[key] = feature;
}

std::string Features::stringnify() {
  std::string ret = "[";
  int64_t i = 0;
  for (auto &kv : m_features) {
    if (i) {
      ret.append(",");
    }
    ret.append("{\"" + kv.first + "\":");
    ret.append(parameter_stringnify(kv.second));
    ret.append("}");
    i++;
  }
  ret.append("]");
  return ret;
}

}  // namespace luban