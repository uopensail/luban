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
  int length;
} SlotMeta;

#pragma pack(push)
#pragma pack(1)
typedef struct Entity {
  size_t slot;     // feature slot id
  size_t size;     // length of data
  int64_t data[];  // feature data
} Entity;

typedef struct EntityArray {
  size_t size;  // length of array
  Entity *array[];
} EntityArray;

#pragma pack(pop)

// create an entity
static Entity *new_entity(size_t size, int64_t slot) {
  auto entity = (Entity *)calloc(1, sizeof(Entity) + sizeof(int64_t) * size);
  entity->slot = slot;
  entity->size = size;
  return entity;
}
static void del_entity(Entity *entity) {
  if (entity != nullptr) {
    free(entity);
    entity = nullptr;
  }
}

static EntityArray *new_entity_array(size_t size) {
  EntityArray *array =
      (EntityArray *)calloc(1, sizeof(EntityArray) + sizeof(Entity *) * size);
  array->size = size;
  return array;
}

static void del_entity_array(EntityArray *array) {
  if (array != nullptr) {
    for (size_t i = 0; i < array->size; i++) {
      del_entity(array->array[i]);
      array->array[i] = nullptr;
    }
    free(array);
    array = nullptr;
  }
}

static size_t get_entity_size(const Entity *entity) {
  if (entity == nullptr) {
    return 0;
  }

  return sizeof(Entity) + sizeof(int64_t) * entity->size;
}

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
  Entity *hash_float_feature(SharedFeaturePtr &feature, SlotMeta &meta) {
    assert(feature->has_float_list());
    int64_t v;
    int size = std::min<int>(feature->float_list().value_size(), meta.length);
    Entity *entity = new_entity(meta.length, meta.slot);
    for (int i = 0; i < size; i++) {
      v = mmh3(std::to_string(
          static_cast<int64_t>(floorf(feature->float_list().value(i)))));
      entity->data[i] = mask_slot(v, meta.slot);
    }
    return entity;
  }

  Entity *hash_string_feature(SharedFeaturePtr feature, SlotMeta &meta) {
    assert(feature->has_bytes_list());
    int64_t v;
    int size = std::min<int>(feature->bytes_list().value_size(), meta.length);
    Entity *entity = new_entity(meta.length, meta.slot);

    for (int i = 0; i < size; i++) {
      v = mmh3(feature->bytes_list().value(i));
      entity->data[i] = mask_slot(v, meta.slot);
    }
    return entity;
  }

  Entity *hash_int_feature(SharedFeaturePtr feature, SlotMeta &meta) {
    assert(feature->has_int64_list());
    int64_t v;
    int size = std::min<int>(feature->int64_list().value_size(), meta.length);
    Entity *entity = new_entity(meta.length, meta.slot);
    for (int i = 0; i < size; i++) {
      v = mmh3(std::to_string(feature->int64_list().value(i)));
      entity->data[i] = mask_slot(v, meta.slot);
    }
    return entity;
  }

  Entity *hash_feature(SharedFeaturePtr feature, SlotMeta &meta) {
    switch (feature->kind_case()) {
      case sample::Feature::KindCase::kBytesList:
        return hash_string_feature(feature, meta);
      case sample::Feature::KindCase::kFloatList:
        return hash_float_feature(feature, meta);
      case sample::Feature::KindCase::kInt64List:
        return hash_int_feature(feature, meta);
      default:
        return nullptr;
    }
  }

 public:
  FeatureHashToolkit(
      const std::unordered_map<std::string, SlotMeta> &feature_slot_map) {
    for (auto &kv : feature_slot_map) {
      feature_slot_map_[kv.first] = kv.second;
    }
  }
  ~FeatureHashToolkit() { feature_slot_map_.clear(); };
  EntityArray *call(
      const std::unordered_map<std::string, SharedFeaturePtr> &features) {
    EntityArray *entity_array = new_entity_array(feature_slot_map_.size());
    size_t index = 0;
    for (auto &kv : this->feature_slot_map_) {
      auto it = features.find(kv.first);
      if (it != features.end()) {
        entity_array->array[index] = hash_feature(it->second, kv.second);
        index++;
      }
    }
    entity_array->size = index;
    return entity_array;
  }

 private:
  std::unordered_map<std::string, SlotMeta> feature_slot_map_;
};

static void print_entity(Entity *entity) {
  std::cout << "[";
  if (entity == nullptr) {
    std::cout << "]" << std::endl;
    return;
  }
  for (size_t i = 0; i < entity->size; i++) {
    std::cout << entity->data[i];
    if (i != entity->size - 1) {
      std::cout << ",";
    }
  }
  std::cout << "]" << std::endl;
}
#endif  // LUBAN_FEATURE_HASH_TOOLKIT_HPP