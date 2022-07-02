#ifndef LUBAN_UTILS_H
#define LUBAN_UTILS_H

#include "MurmurHash3.h"
#include "feature.pb.h"
#include "helper.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <sys/time.h>
#include <time.h>
#include <vector>

#ifdef __APPLE__
#define u_int64_t __uint64_t
#elif __linux__
#define u_int64_t uint64_t
#endif

#define FEATURE_GID_BITS 8
#define MAX_GID 256
#define FEAUTRE_HASH_MASK 0xFFFFFFFFFFFFFFul

//单特征参数
struct SingleFuncParams
{
    std::string key;
    std::string func;
    ParamsHelper params;
};

//交叉特征参数
struct CrossFuncParams
{
    std::string keyA;
    std::string keyB;
    std::string func;
    ParamsHelper params;
};

//数据的类型
enum EntityType : int
{
    eNumerical = 1,
    eCategorical = 2,
    eNotSet = 0
};

#pragma pack(push)
#pragma pack(1)
struct Entity
{
    EntityType type;
    size_t gid;  //特征组id
    size_t size; //返回结果的长度
    union
    {
        u_int64_t index[];
        float data[];
    };
};
#pragma pack(pop)

//把gid添加到hashid中的前8位
static u_int64_t mask_gid(u_int64_t hash_id, int gid)
{
    assert(gid >= 0 && gid < MAX_GID);
    return (u_int64_t(gid) << (64 - FEATURE_GID_BITS)) + hash_id & FEAUTRE_HASH_MASK;
}

static void safe_delete(void *ptr)
{
    if (ptr != nullptr)
    {
        free(ptr);
        ptr = nullptr;
    }
}

//字符串split
static void split(const std::string &s, std::vector<std::string> &tockens, const std::string &delimiters = " ")
{
    std::string::size_type lastPos = s.find_first_not_of(delimiters, 0);
    std::string::size_type pos = s.find_first_of(delimiters, lastPos);
    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        tockens.emplace_back(s.substr(lastPos, pos - lastPos));
        lastPos = s.find_first_not_of(delimiters, pos);
        pos = s.find_first_of(delimiters, lastPos);
    }
}

// hash
static u_int64_t mmh3(const std::string &key)
{
    u_int64_t ret[2];
    MurmurHash3_x64_128(key.c_str(), key.size(), 0, &ret);
    return ret[0];
}

static u_int64_t mmh3(const std::string &&key)
{
    u_int64_t ret[2];
    MurmurHash3_x64_128(key.c_str(), key.size(), 0, &ret);
    return ret[0];
}

//获得当前的时间戳
static u_int64_t get_current_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (u_int64_t)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
//新建entity
static Entity *new_entity(EntityType type, size_t size, size_t gid)
{
    Entity *entity = nullptr;
    switch (type)
    {
    case EntityType::eCategorical:
        entity = (Entity *)malloc(sizeof(Entity) + sizeof(u_int64_t) * size);
        break;
    case EntityType::eNumerical:
        entity = (Entity *)malloc(sizeof(Entity) + sizeof(float) * size);
        break;
    default:
        return entity;
    }
    entity->type = type;
    entity->size = size;
    entity->gid = gid;
    return entity;
}

static size_t get_entity_size(const Entity *entity)
{
    if (entity == nullptr)
    {
        return 0;
    }
    switch (entity->type)
    {
    case EntityType::eCategorical:
        return sizeof(Entity) + sizeof(u_int64_t) * entity->size;
    case EntityType::eNumerical:
        return sizeof(Entity) + sizeof(float) * entity->size;
    default:
        return 0;
    }
}

//打印数据
static void print_entity(Entity *entity)
{
    std::cout << "[";
    if (entity == nullptr)
    {
        std::cout << "]" << std::endl;
        return;
    }
    if (entity->type == EntityType::eNumerical)
    {
        for (size_t i = 0; i < entity->size; i++)
        {
            std::cout << entity->data[i];
            if (i != entity->size - 1)
            {
                std::cout << ",";
            }
        }
    }
    else if (entity->type == EntityType::eNumerical)
    {
        for (size_t i = 0; i < entity->size; i++)
        {
            std::cout << entity->index[i];
            if (i != entity->size - 1)
            {
                std::cout << ",";
            }
        }
    }
    std::cout << "]" << std::endl;
}
#endif // LUBAN_UTILS_H