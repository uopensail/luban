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

#ifndef LUBAN_FEATURE_HASH_TOOLKIT_HPP
#define LUBAN_FEATURE_HASH_TOOLKIT_HPP

#pragma once
#include <math.h>

#include <string>
#include <unordered_map>
#include <vector>

#include "MurmurHash3.h"
#include "feature_helper.hpp"

#define MAX_SLOT 128
#define FEATURE_SLOT_BITS 8
#define FEAUTRE_HASH_MASK 0xFFFFFFFFFFFFFFll

typedef struct SlotMeta {
  int64_t slot;
  size_t length;
  std::string name;
} SlotMeta;

// hash
static int64_t mmh3(const std::string &key) {
  char ret[16];
  MurmurHash3_x64_128(key.c_str(), key.size(), 0, ret);
  return ((int64_t *)ret)[0] & FEAUTRE_HASH_MASK;
}

static int64_t mask_slot(int64_t hash, size_t slot) {
  assert(slot >= 0 && slot < MAX_SLOT);
  int64_t ret = slot;
  ret <<= (64 - FEATURE_SLOT_BITS);
  ret |= (hash & FEAUTRE_HASH_MASK);
  return ret;
}

class FeatureHashToolkit {
 private:
  void hash_float_feature(SharedFeaturePtr &feature, SlotMeta &meta,
                          int64_t *array) {
    assert(feature->has_float_list());
    size_t offset = this->indeces_[meta.slot];
    int64_t v;
    size_t size =
        std::min<size_t>(feature->float_list().value_size(), meta.length);
    for (size_t i = 0; i < size; i++) {
      v = mmh3(std::to_string(
          static_cast<int64_t>(floorf(feature->float_list().value(i)))));
      array[offset + i] = mask_slot(v, meta.slot);
    }
  }

  void hash_string_feature(SharedFeaturePtr feature, SlotMeta &meta,
                           int64_t *array) {
    assert(feature->has_bytes_list());
    size_t offset = this->indeces_[meta.slot];
    int64_t v;
    size_t size =
        std::min<size_t>(feature->bytes_list().value_size(), meta.length);
    for (size_t i = 0; i < size; i++) {
      v = mmh3(feature->bytes_list().value(i));
      array[offset + i] = mask_slot(v, meta.slot);
    }
  }

  void hash_int_feature(SharedFeaturePtr feature, SlotMeta &meta,
                        int64_t *array) {
    assert(feature->has_int64_list());
    size_t offset = this->indeces_[meta.slot];
    int64_t v;
    size_t size =
        std::min<size_t>(feature->int64_list().value_size(), meta.length);
    for (size_t i = 0; i < size; i++) {
      v = mmh3(std::to_string(feature->int64_list().value(i)));
      array[offset + i] = mask_slot(v, meta.slot);
    }
  }

  void hash_feature(SharedFeaturePtr feature, SlotMeta &meta, int64_t *array) {
    switch (feature->kind_case()) {
      case sample::Feature::KindCase::kBytesList:
        hash_string_feature(feature, meta, array);
        return;
      case sample::Feature::KindCase::kFloatList:
        hash_float_feature(feature, meta, array);
        return;
      case sample::Feature::KindCase::kInt64List:
        hash_int_feature(feature, meta, array);
        return;
      default:
        return;
    }
  }

 public:
  FeatureHashToolkit(const std::vector<SlotMeta> &features) {
    this->width_ = 0;
    this->indeces_ = (size_t *)calloc(features.size(), sizeof(size_t));
    for (size_t i = 0; i < features.size(); i++) {
      this->indeces_[i] = this->width_;
      this->width_ += features[i].length;
      this->features_.push_back(features[i]);
    }
  }
  ~FeatureHashToolkit() { free(this->indeces_); };

  void call(const std::unordered_map<std::string, SharedFeaturePtr> &features,
            int64_t *array) {
    for (size_t i = 0; i < this->features_.size(); i++) {
      auto it = features.find(this->features_[i].name);
      if (it != features.end()) {
        hash_feature(it->second, this->features_[i], array);
      }
    }
    return;
  }

  size_t width() { return this->width_; }

 private:
  size_t width_;
  size_t *indeces_;
  std::vector<SlotMeta> features_;
};

#endif  // LUBAN_FEATURE_HASH_TOOLKIT_HPP