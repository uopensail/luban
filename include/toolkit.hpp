#ifndef LUBAN_TOOLKIT_H
#define LUBAN_TOOLKIT_H

#include <memory>
#pragma once

#include "feature_hash_toolkit.hpp"
#include "feature_operator_toolkit.hpp"

class Toolkit {
private:
  std::shared_ptr<FeatureHashToolkit> hasher_;
  std::shared_ptr<FeatureOperatorToolkit> operator_;
  std::vector<ConfigureOperator> operator_configs_;

public:
  Toolkit() = delete;
  Toolkit(const Toolkit &) = delete;
  Toolkit(const Toolkit &&) = delete;
  Toolkit(const std::string &config_file);

  void process_bytes(char *features, int len, EntityArray **entity_array) {
    SharedFeaturesPtr tf_features = std::make_shared<tensorflow::Features>();
    tf_features->ParseFromArray(features, len);
    RunTimeFeatures rt_features(tf_features);
    for (auto &o : operator_configs_) {
      this->operator_->call(o, rt_features);
    }
    this->hasher_->hash(rt_features.get_selected(), entity_array);
  }

  void process(tensorflow::Features &features, EntityArray **entity_array) {
    SharedFeaturesPtr tf_features{(tensorflow::Features *)(&features),
                                  delete_do_nothing};

    RunTimeFeatures rt_features(tf_features);
    for (auto &o : operator_configs_) {
      this->operator_->call(o, rt_features);
    }
    this->hasher_->hash(rt_features.get_selected(), entity_array);
  }

  ~Toolkit() {}
};

#endif // LUBAN_TOOLKIT_H
