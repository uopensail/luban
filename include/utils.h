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
#include <tuple>
#include <type_traits>
#include <vector>
#include <iostream>

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
    int gid;  //特征组id
    int size; //返回结果的长度
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
void split(const std::string &s, std::vector<std::string> &tockens, const std::string &delimiters = " ");
// hash
u_int64_t mmh3(const std::string &key);
u_int64_t mmh3(const std::string &&key);

//获得当前的时间戳
u_int64_t get_current_time();
//新建entity
Entity *new_entity(EntityType type, int size, int gid);
size_t get_entity_size(const Entity *entity);
static void print_entity( Entity *entity){
    std::cout << "[";
    if (entity == nullptr){
        std::cout<<"]"<<std::endl;
        return;
    }
    if (entity->type == 1){
        for (int i = 0; i < entity->size;i++){
            std::cout << entity->data[i]<<" ";
        }
    }else{
        for (int i = 0; i < entity->size; i++)
        {
            std::cout << entity->index[i] << " ";
        }
    }
    std::cout << "]" << std::endl;
}
#endif // LUBAN_UTILS_H