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

#ifndef LUBAN_FEATURE_HASH_TOOLKIT_HPP
#define LUBAN_FEATURE_HASH_TOOLKIT_HPP

#include "feature_helper.hpp"
#include "utils.h"
#include <math.h>
#include <string>
#include <unordered_map>
#include <vector>

#pragma once

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
  size_t gid;  //特征组id
  size_t size; //返回结果的长度
  u_int64_t data[];
} Entity;

typedef struct EntityArray {
  size_t size; //返回结果的长度
  Entity *array[];
} EntityArray;

#pragma pack(pop)

//新建entity
static Entity *new_entity(size_t size, size_t gid) {
  return (Entity *)malloc(sizeof(Entity) + sizeof(u_int64_t) * size);
}
static void del_entity(Entity *entity) {
  if (entity != nullptr) {
    free(entity);
  }
}

//新建entity
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
    }
    free(array);
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

//把gid添加到hashid中的前8位
static u_int64_t mask_gid(u_int64_t &hash_id, u_int64_t &gid) {
  assert(gid >= 0 && gid < MAX_GID);
  return (gid << (64 - FEATURE_GID_BITS)) + (hash_id & FEAUTRE_HASH_MASK);
}
// u_int64_t mmh3(const std::string &&key) {}
class FeatureHashToolkit {
private:
  void hash_float_feature(const tensorflow::Feature &feature, Entity **entity,
                          u_int64_t &gid) {
    assert(feature.has_float_list());
    u_int64_t v;
    *entity = new_entity(feature.float_list().value_size(), gid);

    for (int i = 0; i < feature.float_list().value_size(); i++) {
      v = mmh3(std::to_string(
          static_cast<int64_t>(floorf(feature.float_list().value(i)))));
      (*entity)->data[i] = mask_gid(v, gid);
    }
  }

  void hash_string_feature(const tensorflow::Feature &feature, Entity **entity,
                           u_int64_t &gid) {
    assert(feature.has_bytes_list());
    u_int64_t v;
    *entity = new_entity(feature.bytes_list().value_size(), gid);

    for (int i = 0; i < feature.bytes_list().value_size(); i++) {
      v = mmh3(feature.bytes_list().value(i));
      (*entity)->data[i] = mask_gid(v, gid);
    }
  }

  void hash_int_feature(const tensorflow::Feature &feature, Entity **entity,
                        u_int64_t &gid) {
    assert(feature.has_int64_list());
    u_int64_t v;
    *entity = new_entity(feature.int64_list().value_size(), gid);
    for (int i = 0; i < feature.int64_list().value_size(); i++) {
      v = mmh3(std::to_string(feature.int64_list().value(i)));
      (*entity)->data[i] = mask_gid(v, gid);
    }
  }

  void hash_feature(const tensorflow::Feature &feature, Entity **entity,
                    u_int64_t &gid) {
    switch (feature.kind_case()) {
    case tensorflow::Feature::KindCase::kBytesList:
      hash_int_feature(feature, entity, gid);
      return;
    case tensorflow::Feature::KindCase::kFloatList:
      hash_float_feature(feature, entity, gid);
      return;
    case tensorflow::Feature::KindCase::kInt64List:
      hash_int_feature(feature, entity, gid);
      return;
    default:
      *entity = nullptr;
      return;
    }
  }

  u_int64_t *get_gid(const std::string &key) {
    auto iter = feature_gid_map_.find(key);
    if (iter != feature_gid_map_.end()) {
      return &iter->second;
    }
    return nullptr;
  }

public:
  FeatureHashToolkit(
      std::unordered_map<std::string, u_int64_t> &feature_gid_map) {
    for (auto &kv : feature_gid_map) {
      feature_gid_map_[kv.first] = kv.second;
    }
  }
  ~FeatureHashToolkit() { feature_gid_map_.clear(); };
  void hash(SharedFeaturesPtr features, EntityArray **entity_array) {
    *entity_array = new_entity_array(features->feature_size());
    auto &fea_map = features->feature();
    u_int64_t *ptr = nullptr;
    int index = 0;
    for (auto &kv : fea_map) {
      ptr = this->get_gid(kv.first);
      if (ptr == nullptr) {
        (*entity_array)->array[index] = nullptr;
      } else {
        hash_feature(kv.second, &((*entity_array)->array[index]), *ptr);
      }
    }
  }

private:
  std::unordered_map<std::string, u_int64_t> feature_gid_map_;
};

//打印数据
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