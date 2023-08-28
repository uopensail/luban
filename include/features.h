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

#ifndef LUBAN_FEATURES_H
#define LUBAN_FEATURES_H

#include "common.h"

namespace luban {

class Features {
 public:
  Features() = default;
  Features(std::string_view value);
  Features(const std::string &value);
  Features(const std::initializer_list<std::string_view> &values);
  Features(const std::initializer_list<std::string> &values);
  Features(const std::vector<std::string_view> &values);
  Features(const std::vector<std::string> &values);
  Features &operator=(const Features &features);
  ~Features() = default;
  SharedParameter operator[](const std::string &key);
  void set(const std::string &key, SharedParameter feature);
  std::string stringnify();

 private:
  SharedParameterMap m_features;
};

using SharedFeaturesPtr = std::shared_ptr<Features>;

class FeaturesList {
 public:
  FeaturesList() = default;
  FeaturesList &operator=(const FeaturesList &);
  ~FeaturesList() = default;
  void push(SharedFeaturesPtr features);
  SharedFeaturesPtr operator[](size_t index) const;
  void set(size_t index, SharedFeaturesPtr features);
  int64_t size() const;
  std::string stringnify();

 private:
  std::vector<SharedFeaturesPtr> m_featureslist;
};

using SharedFeaturesListPtr = std::shared_ptr<FeaturesList>;

}  // namespace luban

#endif  // LUBAN_FEATURES_H