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

#ifndef LUBAN_TOOLKIT_HPP
#define LUBAN_TOOLKIT_HPP

#pragma once
#include <memory>

#include "hash_toolkit.hpp"
#include "operator_toolkit.hpp"
#include "runtime.hpp"

class Toolkit {
 private:
  EntityArray *process_from_realtime_features(
      RunTimeFeatures &realtime_features) {
    this->operator_->call(realtime_features);
    return this->hasher_->call(realtime_features.get_features());
  }

 public:
  Toolkit() = delete;
  Toolkit(const Toolkit &) = delete;
  Toolkit(const Toolkit &&) = delete;
  Toolkit(const std::string &config_file) {
    std::shared_ptr<cpptoml::table> g = cpptoml::parse_file(config_file);

    // get the operator configures from toml file
    std::vector<ConfigureOperator> operator_configs;
    auto transforms = g->get_table_array("transforms");
    for (const auto &table : *transforms) {
      operator_configs.push_back({table});
    }
    this->operator_ =
        std::make_shared<FeatureOperatorToolkit>(operator_configs);

    std::unordered_map<std::string, SlotMeta> feature_slot_map;
    auto groups = g->get_table_array("outputs");
    int length;
    int64_t slot;
    for (auto &table : *groups) {
      assert(table->contains("name") && table->contains("slot"));
      ParamsHelper params(table);
      slot = params.get<int64_t>("slot");
      length = params.get<int>("length", 1);
      feature_slot_map[params.get<std::string>("name")] =
          SlotMeta{slot, length};
    }
    this->hasher_ = std::make_shared<FeatureHashToolkit>(feature_slot_map);
  }

  EntityArray *process(char *features, int len) {
    sample::Features *tf_features = new sample::Features();
    tf_features->ParseFromArray(features, len);
    RunTimeFeatures rt_features(tf_features);
    auto ret = this->process_from_realtime_features(rt_features);
    delete tf_features;
    return ret;
  }

  EntityArray *process(sample::Features *features) {
    RunTimeFeatures rt_features(features);
    return this->process_from_realtime_features(rt_features);
  }

  EntityArray *process(
      const std::initializer_list<sample::Features *> &features_list) {
    RunTimeFeatures rt_features(features_list);
    return this->process_from_realtime_features(rt_features);
  }

  ~Toolkit() {}

 private:
  std::shared_ptr<FeatureHashToolkit> hasher_;
  std::shared_ptr<FeatureOperatorToolkit> operator_;
};

#endif  // LUBAN_TOOLKIT_HPP