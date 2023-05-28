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

#ifdef __APPLE__
#define u_int64_t __uint64_t
#elif __linux__
#define u_int64_t uint64_t
#endif

#define FEATURE_GID_BITS 8
#define MAX_GID 256
#define FEAUTRE_HASH_MASK 0xFFFFFFFFFFFFFFul

#pragma pack(push)
#pragma pack(1)
typedef struct Entity {
  size_t gid;  // feature group id
  size_t size; // length of data
  u_int64_t data[];
} Entity;

typedef struct EntityArray {
  size_t size; // length of array
  Entity *array[];
} EntityArray;

#pragma pack(pop)

// create an entity
static Entity *new_entity(size_t size, size_t gid) {
  auto entity = (Entity *)malloc(sizeof(Entity) + sizeof(u_int64_t) * size);
  entity->gid = gid;
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

  return sizeof(Entity) + sizeof(u_int64_t) * entity->size;
}

// hash
static u_int64_t mmh3(const std::string &key) {
  u_int64_t ret[2];
  MurmurHash3_x64_128(key.c_str(), key.size(), 0, &ret);
  return ret[0];
}

static u_int64_t mask_gid(u_int64_t &hash_id, u_int64_t &gid) {
  assert(gid >= 0 && gid < MAX_GID);
  return (gid << (64 - FEATURE_GID_BITS)) + (hash_id & FEAUTRE_HASH_MASK);
}

class FeatureHashToolkit {
private:
  Entity *hash_float_feature(SharedFeaturePtr &feature, u_int64_t gid) {
    assert(feature->has_float_list());
    u_int64_t v;
    Entity *entity = new_entity(feature->float_list().value_size(), gid);

    for (int i = 0; i < feature->float_list().value_size(); i++) {
      v = mmh3(std::to_string(
          static_cast<int64_t>(floorf(feature->float_list().value(i)))));
      entity->data[i] = mask_gid(v, gid);
    }
    return entity;
  }

  Entity *hash_string_feature(SharedFeaturePtr feature, u_int64_t gid) {
    assert(feature->has_bytes_list());
    u_int64_t v;
    Entity *entity = new_entity(feature->bytes_list().value_size(), gid);

    for (int i = 0; i < feature->bytes_list().value_size(); i++) {
      v = mmh3(feature->bytes_list().value(i));
      entity->data[i] = mask_gid(v, gid);
    }
    return entity;
  }

  Entity *hash_int_feature(SharedFeaturePtr feature, u_int64_t gid) {
    assert(feature->has_int64_list());
    u_int64_t v;
    Entity *entity = new_entity(feature->int64_list().value_size(), gid);
    for (int i = 0; i < feature->int64_list().value_size(); i++) {
      v = mmh3(std::to_string(feature->int64_list().value(i)));
      entity->data[i] = mask_gid(v, gid);
    }
    return entity;
  }

  Entity *hash_feature(SharedFeaturePtr feature, u_int64_t gid) {
    switch (feature->kind_case()) {
    case sample::Feature::KindCase::kBytesList:
      return hash_string_feature(feature, gid);
    case sample::Feature::KindCase::kFloatList:
      return hash_float_feature(feature, gid);
    case sample::Feature::KindCase::kInt64List:
      return hash_int_feature(feature, gid);
    default:
      return nullptr;
    }
  }

  u_int64_t *get_gid(const std::string &key) {
    auto iter = feature_group_map_.find(key);
    if (iter != feature_group_map_.end()) {
      return &iter->second;
    }
    return nullptr;
  }

public:
  FeatureHashToolkit(
      const std::unordered_map<std::string, u_int64_t> &feature_group_map) {
    for (auto &kv : feature_group_map) {
      feature_group_map_[kv.first] = kv.second;
    }
  }
  ~FeatureHashToolkit() { feature_group_map_.clear(); };
  EntityArray *
  call(const std::unordered_map<std::string, SharedFeaturePtr> &features) {
    EntityArray *entity_array = new_entity_array(feature_group_map_.size());
    int index = 0;

    for (const auto &kv : feature_group_map_) {
      auto it = features.find(kv.first);
      if (it == features.end()) {
        entity_array->array[index] = nullptr;
      } else {
        entity_array->array[index] = hash_feature(it->second, kv.second);
      }
      index++;
    }
    return entity_array;
  }

private:
  std::unordered_map<std::string, u_int64_t> feature_group_map_;
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
#endif // LUBAN_FEATURE_HASH_TOOLKIT_HPP