#include "feature.h"

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

void Features::set(const std::string &key, SharedParameter feature) {
  m_features[key] = feature;
}

void Features::merge(SharedFeaturesPtr features) {
  m_features.merge(features->m_features);
}
std::string Features::stringnify() {
  json j;
  for (auto &kv : m_features) {
    if (auto *p = std::get_if<int64_t>(kv.second.get())) {
      j[kv.first]["value"] = *p;
      j[kv.first]["type"] = DataType::kInt64;
    } else if (auto *p = std::get_if<float>(kv.second.get())) {
      j[kv.first]["value"] = *p;
      j[kv.first]["type"] = DataType::kFloat32;
    } else if (auto *p = std::get_if<std::string>(kv.second.get())) {
      j[kv.first]["value"] = *p;
      j[kv.first]["type"] = DataType::kString;
    } else if (auto *p = std::get_if<std::vector<int64_t>>(kv.second.get())) {
      j[kv.first]["value"] = *p;
      j[kv.first]["type"] = DataType::kFloat32Array;
    } else if (auto *p = std::get_if<std::vector<float>>(kv.second.get())) {
      j[kv.first]["value"] = *p;
      j[kv.first]["type"] = DataType::kFloat32Array;
    } else if (auto *p =
                   std::get_if<std::vector<std::string>>(kv.second.get())) {
      j[kv.first]["value"] = *p;
      j[kv.first]["type"] = DataType::kStringArray;
    }
  }
  return j.dump();
}

FeaturesList &FeaturesList::operator=(const FeaturesList &list) {
  if (this == &list) {
    return *this;
  }

  m_featureslist.clear();
  m_featureslist.assign(list.m_featureslist.begin(), list.m_featureslist.end());
  return *this;
}

void FeaturesList::push(SharedFeaturesPtr features) {
  m_featureslist.push_back(features);
}

SharedFeaturesPtr FeaturesList::operator[](size_t index) const {
  assert(index >= 0 && index < m_featureslist.size());
  return m_featureslist[index];
}

void FeaturesList::set(size_t index, SharedFeaturesPtr features) {
  assert(index >= 0 && index < m_featureslist.size());
  m_featureslist[index] = features;
}

int64_t FeaturesList::size() const { return m_featureslist.size(); }

std::string FeaturesList::stringnify() {
  std::string ret = "[";
  for (size_t i = 0; i < m_featureslist.size(); ++i) {
    if (i > 0) {
      ret.append(", ");
    }
    ret.append(m_featureslist[i]->stringnify());
  }
  ret.append("]");
  return ret;
}

} // namespace luban