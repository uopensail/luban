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
  EntityArray *
  process_from_realtime_features(RunTimeFeatures &realtime_features) {
    for (const auto &o : operator_configs_) {
      this->operator_->call(o, realtime_features);
    }
    return this->hasher_->call(realtime_features.get_selected());
  }

public:
  Toolkit() = delete;
  Toolkit(const Toolkit &) = delete;
  Toolkit(const Toolkit &&) = delete;
  Toolkit(const std::string &config_file) {
    std::string cfg_toml =
        config_file.substr(0, config_file.size() - 4) + "toml";
    std::string command =
        "/usr/local/bin/luban_parser -i " + config_file + " -o " + cfg_toml;
    system(command.c_str());
    std::shared_ptr<cpptoml::table> g = cpptoml::parse_file(cfg_toml);

    // get the operator configures from toml file
    auto operators = g->get_table_array("operators");
    for (const auto &table : *operators) {
      operator_configs_.push_back({table});
    }

    auto groups = g->get_table_array("groups");
    for (auto &table : *groups) {
      assert(table->contains("name") && table->contains("gid"));
      ParamsHelper params(table);
      feature_gid_map_[params.get<std::string>("name")] =
          (u_int64_t)params.get<int64_t>("gid");
    }
    this->hasher_ = std::make_shared<FeatureHashToolkit>(feature_gid_map_);
    this->operator_ =
        std::make_shared<FeatureOperatorToolkit>(operator_configs_);
  }

  EntityArray *process(char *features, int len) {
    sample::Features *tf_features = new sample::Features();
    tf_features->ParseFromArray(features, len);
    RunTimeFeatures rt_features(tf_features);
    auto *array = this->process_from_realtime_features(rt_features);
    delete tf_features;
    return array;
  }

  EntityArray *process(sample::Features *features) {
    RunTimeFeatures rt_features(features);
    auto *array = this->process_from_realtime_features(rt_features);
    return array;
  }

  EntityArray *
  process(const std::initializer_list<sample::Features *> &features_list) {
    RunTimeFeatures rt_features(features_list);
    return this->process_from_realtime_features(rt_features);
  }

  ~Toolkit() {}

private:
  std::shared_ptr<FeatureHashToolkit> hasher_;
  std::shared_ptr<FeatureOperatorToolkit> operator_;
  std::vector<ConfigureOperator> operator_configs_;
  std::unordered_map<std::string, u_int64_t> feature_gid_map_;
};

#endif // LUBAN_TOOLKIT_HPP
