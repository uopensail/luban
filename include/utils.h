#ifndef LUBAN_UTILS_H
#define LUBAN_UTILS_H

#pragma once

#include "MurmurHash3.h"
#include "feature.pb.h"
#include "helper.h"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <sys/time.h>
#include <time.h>
#include <typeinfo>
#include <vector>

#ifdef __APPLE__
#define u_int64_t __uint64_t
#elif __linux__
#define u_int64_t uint64_t
#endif

#define FEATURE_GID_BITS 8
#define MAX_GID 256
#define FEAUTRE_HASH_MASK 0xFFFFFFFFFFFFFFul

//判断类型
#define is_int(T) (std::is_same<T, long long>::value || std::is_same<T, int>::value || std::is_same<T, long>::value || std::is_same<T, unsigned long long>::value)
#define is_float(T) (std::is_same<T, float>::value || std::is_same<T, double>::value)
#define is_str(T) (std::is_same<T, std::string>::value)

//定义特征的智能指针类型
#define SharedFeaturePtr std::shared_ptr<tensorflow::Feature>
#define SharedFeaturesPtr std::shared_ptr<tensorflow::Features>

//打印模板类型，用于调试
template <typename T>
constexpr std::string_view type_name()
{
    std::string_view name, prefix, suffix;
#ifdef __clang__
    name = __PRETTY_FUNCTION__;
    prefix = "auto type_name() [T = ";
    suffix = "]";
#elif defined(__GNUC__)
    name = __PRETTY_FUNCTION__;
    prefix = "constexpr auto type_name() [with T = ";
    suffix = "]";
#elif defined(_MSC_VER)
    name = __FUNCSIG__;
    prefix = "auto __cdecl type_name<";
    suffix = ">(void)";
#endif
    name.remove_prefix(prefix.size());
    name.remove_suffix(suffix.size());
    return name;
}

#define PRINT_TYPE(X) std::cout << type_name<X>() << std::endl;

template <typename T>
void print_template_type()
{
    PRINT_TYPE(T);
}

template <typename T1, typename T2, typename... Ts>
void print_template_type()
{
    if (sizeof...(Ts) == 0)
    {
        PRINT_TYPE(T1);
        PRINT_TYPE(T2);
    }
    else
    {
        PRINT_TYPE(T1);
        print_template_type<T2, Ts...>();
    }
}

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
u_int64_t mask_gid(u_int64_t hash_id, int gid);

void safe_delete(void *ptr);

//字符串split
void split(const std::string &s, std::vector<std::string> &tokens, const std::string &delimiters = " ");

// hash
u_int64_t mmh3(const std::string &key);
u_int64_t mmh3(const std::string &&key);

//获得当前的时间戳
u_int64_t get_current_time();

//新建entity
Entity *new_entity(EntityType type, size_t size, size_t gid);
size_t get_entity_size(const Entity *entity);

//打印数据
void print_entity(Entity *entity);

void split_file(std::string filename, int num);
void write_record(std::ofstream &writer, int label, std::vector<u_int64_t> &ret);
int get_label(const tensorflow::Features &features);
#endif // LUBAN_UTILS_H