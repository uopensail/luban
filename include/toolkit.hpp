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
  void process_from_realtime_features(RunTimeFeatures &realtime_features,
                                      int64_t *array) {
    this->operator_->call(realtime_features);
    this->hasher_->call(realtime_features.get_features(), array);
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

    auto groups = g->get_table_array("outputs");
    std::vector<SlotMeta> features;
    features.resize(groups->get().size());
    size_t length;
    int64_t slot;
    for (auto &table : *groups) {
      assert(table->contains("name") && table->contains("slot"));
      ParamsHelper params(table);
      slot = params.get<int64_t>("slot");
      length = params.get<size_t>("length", 1);
      features[slot] = SlotMeta{slot, length, params.get<std::string>("name")};
    }

    this->operator_ =
        std::make_shared<FeatureOperatorToolkit>(operator_configs);

    this->hasher_ = std::make_shared<FeatureHashToolkit>(features);
  }

  void process(char *features, int len, int64_t *array) {
    sample::Features *tf_features = new sample::Features();
    tf_features->ParseFromArray(features, len);
    RunTimeFeatures rt_features(tf_features);
    this->process_from_realtime_features(rt_features, array);
    delete tf_features;
  }

  void process(sample::Features *features, int64_t *array) {
    RunTimeFeatures rt_features(features);
    return this->process_from_realtime_features(rt_features, array);
  }

  void process(const std::initializer_list<sample::Features *> &features_list,
               int64_t *array) {
    RunTimeFeatures rt_features(features_list);
    return this->process_from_realtime_features(rt_features, array);
  }

  ~Toolkit() {}

  size_t width() { return this->hasher_->width(); }

 private:
  std::shared_ptr<FeatureHashToolkit> hasher_;
  std::shared_ptr<FeatureOperatorToolkit> operator_;
};

#endif  // LUBAN_TOOLKIT_HPP