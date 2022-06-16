#ifndef LUBAN_UTILS_H
#define LUBAN_UTILS_H

#include "MurmurHash3.h"
#include "feature.pb.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <sys/time.h>
#include <time.h>
#include <tuple>
#include <type_traits>
#include <vector>

#define slot_shift 56
#define slot_mask 0xffffffffffffff

#ifdef __APPLE__
#define u_int64_t __uint64_t
#elif __linux__
#define u_int64_t uint64_t
#endif

#define mask(x, y) (((y) << slot_shift) + (slot_mask & (x)))

namespace luban
{
    u_int64_t mmh3(std::string &key);

    //分桶
    int get_bucket(float &value, float &low, float &high, int &buckets);

    u_int64_t mmh3(std::string &&key);

    //特征转成string
    void feature_to_strings(tensorflow::Feature &feature, std::vector<std::string> &list);

    //特征转成float
    void feature_to_floats(tensorflow::Feature &feature, std::vector<float> &list);

    u_int64_t get_current_time();
} // namespace luban

#endif // LUBAN_UTILS_H