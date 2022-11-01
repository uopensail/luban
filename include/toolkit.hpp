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

#ifndef LUBAN_TOOLKIT_HPP
#define LUBAN_TOOLKIT_HPP

#pragma once
#include <memory>

#include "feature_hash_toolkit.hpp"
#include "feature_operator_toolkit.hpp"

class Toolkit {
 private:
  std::shared_ptr<FeatureHashToolkit> hasher_;
  std::shared_ptr<FeatureOperatorToolkit> operator_;
  std::vector<ConfigureOperator> operator_configs_;
  std::unordered_map<std::string, u_int64_t> feature_gid_map_;

 private:
  void process_from_realtime_features(RunTimeFeatures &realtime_features,
                                      EntityArray **entity_array) {
    for (auto &o : operator_configs_) {
      this->operator_->call(o, realtime_features);
    }
    this->hasher_->call(realtime_features.get_selected(), entity_array);
  }

 public:
  Toolkit() = delete;
  Toolkit(const Toolkit &) = delete;
  Toolkit(const Toolkit &&) = delete;
  Toolkit(const std::string &config_file) {
    std::shared_ptr<cpptoml::table> g = cpptoml::parse_file(config_file);

    // get the operator configures from toml file
    auto operators = g->get_table_array("operators");
    for (const auto &table : *operators) {
      operator_configs_.push_back({table});
    }
    //
    auto groups = g->get_table_array("groups");
    for (auto &table : *groups) {
      assert(table->contains("name") && table->contains("gid"));
      ParamsHelper params(table);
      feature_gid_map_[params.get<std::string>("name")] =
          (u_int64_t)params.get<int64_t>("gid");
    }
    this->hasher_ = std::make_shared<FeatureHashToolkit>(feature_gid_map_);
    this->operator_ = std::make_shared<FeatureOperatorToolkit>();
  }

  void process(char *features, int len, EntityArray **entity_array) {
    SharedFeaturesPtr tf_features = std::make_shared<tensorflow::Features>();
    tf_features->ParseFromArray(features, len);
    RunTimeFeatures rt_features(tf_features);
    this->process_from_realtime_features(rt_features, entity_array);
  }

  void process(const tensorflow::Features &features,
               EntityArray **entity_array) {
    RunTimeFeatures rt_features(features);
    this->process_from_realtime_features(rt_features, entity_array);
  }

  void process(const SharedFeaturesPtr &features, EntityArray **entity_array) {
    RunTimeFeatures rt_features(features);
    this->process_from_realtime_features(rt_features, entity_array);
  }

  void process(const std::initializer_list<SharedFeaturesPtr> &features_list,
               EntityArray **entity_array) {
    RunTimeFeatures rt_features(features_list);
    this->process_from_realtime_features(rt_features, entity_array);
  }

  void process(const std::initializer_list<tensorflow::Features> &features_list,
               EntityArray **entity_array) {
    RunTimeFeatures rt_features(features_list);
    this->process_from_realtime_features(rt_features, entity_array);
  }

  void process(
      const std::initializer_list<tensorflow::Features *> &features_list,
      EntityArray **entity_array) {
    RunTimeFeatures rt_features(features_list);
    this->process_from_realtime_features(rt_features, entity_array);
  }

  ~Toolkit() {}
};

#endif  // LUBAN_TOOLKIT_HPP
