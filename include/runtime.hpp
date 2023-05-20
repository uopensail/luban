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
    for (auto &kv : features.origin_) {
      origin_[kv.first] = kv.second;
    }
    for (auto &kv : features.selected_) {
      origin_[kv.first] = kv.second;
    }
    for (auto &kv : features.anonymous_) {
      origin_[kv.first] = kv.second;
    }
  }
  RunTimeFeatures &operator=(const RunTimeFeatures &features) {
    if (this == &features) {
      return *this;
    }
    for (auto &kv : features.origin_) {
      origin_[kv.first] = kv.second;
    }
    for (auto &kv : features.selected_) {
      origin_[kv.first] = kv.second;
    }
    for (auto &kv : features.anonymous_) {
      origin_[kv.first] = kv.second;
    }
    return *this;
  }

  RunTimeFeatures(const sample::Features *features) {
    assert(features != nullptr);
    const auto &fea = features->feature();
    for (auto &kv : fea) {
      origin_[kv.first] =
          SharedFeaturePtr{(sample::Feature *)(&(kv.second)), do_nothing};
    }
  }

  RunTimeFeatures(
      const std::initializer_list<sample::Features *> &features_list) {
    for (auto &it : features_list) {
      assert(it != nullptr);
      const auto &features = it->feature();
      for (auto &kv : features) {
        origin_[kv.first] =
            SharedFeaturePtr{(sample::Feature *)(&(kv.second)), do_nothing};
      }
    }
  }

  RunTimeFeatures(const std::vector<sample::Features *> &features_list) {
    for (auto &it : features_list) {
      assert(it != nullptr);
      const auto &features = it->feature();
      for (auto &kv : features) {
        origin_[kv.first] =
            SharedFeaturePtr{(sample::Feature *)(&(kv.second)), do_nothing};
      }
    }
  }

  ~RunTimeFeatures() {
    this->origin_.clear();
    this->anonymous_.clear();
    this->selected_.clear();
  }

  const std::unordered_map<std::string, SharedFeaturePtr> &get_selected() {
    return this->selected_;
  }

  // insert feature
  void insert(VariableType type, const std::string &key,
              const SharedFeaturePtr &feature) {
    assert(type == VariableType::VT_Anonymous_Feature ||
           type == VariableType::VT_Selected_Feature);
    switch (type) {
    case VariableType::VT_Anonymous_Feature:
      this->anonymous_[key] = feature;
      return;
    case VariableType::VT_Selected_Feature:
      this->selected_[key] = feature;
      return;
    default:
      return;
    }
  }

  // get feature
  SharedFeaturePtr get(const ConfigureParameter &p) {
    const VariableType &type = p.get_type();
    assert(type == VariableType::VT_Anonymous_Feature ||
           type == VariableType::VT_Selected_Feature ||
           type == VariableType::VT_Origin_Feature);
    const std::string *key = (std::string *)p.get();
    switch (type) {
    case VariableType::VT_Anonymous_Feature: {
      auto iter = this->anonymous_.find(*key);
      if (iter != this->anonymous_.end()) {
        return iter->second;
      } else {
        return nullptr;
      }
    }
    case VariableType::VT_Selected_Feature: {
      auto iter = this->selected_.find(*key);
      if (iter != this->selected_.end()) {
        return iter->second;
      } else {
        return nullptr;
      }
    }
    case VariableType::VT_Origin_Feature: {
      auto iter = this->origin_.find(*key);
      if (iter != this->origin_.end()) {
        return iter->second;
      } else {
        return nullptr;
      }
    }
    default:
      return nullptr;
    }
  }

private:
  std::unordered_map<std::string, SharedFeaturePtr> selected_;
  std::unordered_map<std::string, SharedFeaturePtr> anonymous_;
  std::unordered_map<std::string, SharedFeaturePtr> origin_;
};
#endif // LUBAN_FEATURE_OPERATOR_RUNTIME_HPP