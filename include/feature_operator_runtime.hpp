//
// `LuBan` - 'c++ tool for transformating and hashing features'
// Copyright (C) 2019 - present timepi <timepi123@gmail.com>
//
// This file is part of `LuBan`.
// //
// `LuBan` is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// `LuBan` is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with `LuBan`.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef LUBAN_FEATURE_OPERATOR_RUNTIME_HPP
#define LUBAN_FEATURE_OPERATOR_RUNTIME_HPP

#pragma once
#include <initializer_list>
#include <unordered_map>

#include "feature_helper.hpp"
#include "feature_operator_configure.hpp"

//运行时的特征
class RunTimeFeatures {
 private:
  SharedFeaturesPtr selected_;
  SharedFeaturesPtr anonymous_;
  std::unordered_map<std::string, SharedFeaturePtr> origin_;

 public:
  RunTimeFeatures() = delete;
  RunTimeFeatures(const RunTimeFeatures &features)
      : selected_(features.selected_), anonymous_(features.anonymous_) {
    for (auto &kv : features.origin_) {
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
    this->selected_ = features.selected_;
    this->anonymous_ = features.anonymous_;
    return *this;
  }
  RunTimeFeatures(const SharedFeaturesPtr &features) {
    selected_ = std::make_shared<tensorflow::Features>();
    anonymous_ = std::make_shared<tensorflow::Features>();
    const auto &fea = features->feature();
    for (auto &kv : fea) {
      origin_[kv.first] = SharedFeaturePtr{
          (tensorflow::Feature *)(&(kv.second)), delete_do_nothing};
    }
  }

  RunTimeFeatures(const tensorflow::Features &features) {
    selected_ = std::make_shared<tensorflow::Features>();
    anonymous_ = std::make_shared<tensorflow::Features>();
    const auto &fea = features.feature();
    for (auto &kv : fea) {
      origin_[kv.first] = SharedFeaturePtr{
          (tensorflow::Feature *)(&(kv.second)), delete_do_nothing};
    }
  }

  RunTimeFeatures(
      const std::initializer_list<tensorflow::Features> &features_list) {
    for (auto &it : features_list) {
      const auto &features = it.feature();
      for (auto &kv : features) {
        origin_[kv.first] = SharedFeaturePtr{
            (tensorflow::Feature *)(&(kv.second)), delete_do_nothing};
      }
    }
  }

  RunTimeFeatures(
      const std::initializer_list<SharedFeaturesPtr> &features_list) {
    selected_ = std::make_shared<tensorflow::Features>();
    anonymous_ = std::make_shared<tensorflow::Features>();
    for (auto &it : features_list) {
      const auto &features = it->feature();
      for (auto &kv : features) {
        origin_[kv.first] = SharedFeaturePtr{
            (tensorflow::Feature *)(&(kv.second)), delete_do_nothing};
      }
    }
  }

  RunTimeFeatures(const std::vector<SharedFeaturesPtr> &features_list) {
    selected_ = std::make_shared<tensorflow::Features>();
    anonymous_ = std::make_shared<tensorflow::Features>();
    for (auto &it : features_list) {
      const auto &features = it->feature();
      for (auto &kv : features) {
        origin_[kv.first] = SharedFeaturePtr{
            (tensorflow::Feature *)(&(kv.second)), delete_do_nothing};
      }
    }
  }

  RunTimeFeatures(const std::vector<tensorflow::Features> &features_list) {
    selected_ = std::make_shared<tensorflow::Features>();
    anonymous_ = std::make_shared<tensorflow::Features>();
    for (auto &it : features_list) {
      const auto &features = it.feature();
      for (auto &kv : features) {
        origin_[kv.first] = SharedFeaturePtr{
            (tensorflow::Feature *)(&(kv.second)), delete_do_nothing};
      }
    }
  }

  RunTimeFeatures(const std::vector<tensorflow::Features *> &features_list) {
    selected_ = std::make_shared<tensorflow::Features>();
    anonymous_ = std::make_shared<tensorflow::Features>();
    for (auto &it : features_list) {
      const auto &features = it->feature();
      for (auto &kv : features) {
        origin_[kv.first] = SharedFeaturePtr{
            (tensorflow::Feature *)(&(kv.second)), delete_do_nothing};
      }
    }
  }

  ~RunTimeFeatures() { this->origin_.clear(); }

  const SharedFeaturesPtr &get_selected() { return this->selected_; }

  //添加一个值
  void insert(VariableType type, const std::string &key,
              const SharedFeaturePtr &feature) {
    assert(type == VariableType::VT_Anonymous_Feature ||
           type == VariableType::VT_Selected_Feature);
    switch (type) {
      case VariableType::VT_Anonymous_Feature:
        (*this->anonymous_->mutable_feature())[key] = *feature;
        return;
      case VariableType::VT_Selected_Feature:
        (*this->selected_->mutable_feature())[key] = *feature;
        return;
      default:
        return;
    }
  }

  //取某个特征
  SharedFeaturePtr get(VariableType type, const std::string &key) {
    assert(type == VariableType::VT_Anonymous_Feature ||
           type == VariableType::VT_Selected_Feature ||
           type == VariableType::VT_Origin_Feature);
    switch (type) {
      case VariableType::VT_Anonymous_Feature:
        return get_feature_by_key(this->anonymous_, key);
      case VariableType::VT_Selected_Feature:
        return get_feature_by_key(this->selected_, key);
      case VariableType::VT_Origin_Feature: {
        auto iter = this->origin_.find(key);
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
};

#endif  // LUBAN_FEATURE_OPERATOR_RUNTIME_HPP