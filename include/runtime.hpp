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

#ifndef LUBAN_FEATURE_OPERATOR_RUNTIME_HPP
#define LUBAN_FEATURE_OPERATOR_RUNTIME_HPP

#pragma once

#include <initializer_list>
#include <unordered_map>

#include "configure.hpp"
#include "feature_helper.hpp"

static void do_nothing(void *ptr) {}

class RunTimeFeatures {

public:
  RunTimeFeatures() = delete;
  RunTimeFeatures(const RunTimeFeatures &features) {
    for (auto kv : features.features_) {
      features_[kv.first] = kv.second;
    }
  }
  RunTimeFeatures &operator=(const RunTimeFeatures &features) {
    if (this == &features) {
      return *this;
    }
    for (auto kv : features.features_) {
      features_[kv.first] = kv.second;
    }
    return *this;
  }

  RunTimeFeatures(const sample::Features *features) {
    assert(features != nullptr);
    const auto &fea = features->feature();
    for (auto &kv : fea) {
      features_[kv.first] =
          SharedFeaturePtr{(sample::Feature *)(&(kv.second)), do_nothing};
    }
  }

  RunTimeFeatures(
      const std::initializer_list<sample::Features *> &features_list) {
    for (auto &it : features_list) {
      assert(it != nullptr);
      const auto &features = it->feature();
      for (auto &kv : features) {
        features_[kv.first] =
            SharedFeaturePtr{(sample::Feature *)(&(kv.second)), do_nothing};
      }
    }
  }

  RunTimeFeatures(const std::vector<sample::Features *> &features_list) {
    for (auto &it : features_list) {
      assert(it != nullptr);
      const auto &features = it->feature();
      for (auto &kv : features) {
        features_[kv.first] =
            SharedFeaturePtr{(sample::Feature *)(&(kv.second)), do_nothing};
      }
    }
  }

  ~RunTimeFeatures() { features_.clear(); }

  // insert feature
  void insert(const std::string &key, const SharedFeaturePtr &feature) {
    this->features_[key] = feature;
  }

  const std::unordered_map<std::string, SharedFeaturePtr> &get_features() {
    return this->features_;
  }

  // get feature
  SharedFeaturePtr get(const std::string &key) {
    auto iter = this->features_.find(key);
    if (iter != this->features_.end()) {
      return iter->second;
    } else {
      return nullptr;
    }
  }

private:
  std::unordered_map<std::string, SharedFeaturePtr> features_;
};
#endif // LUBAN_FEATURE_OPERATOR_RUNTIME_HPP