#ifndef LUBAN_UTILS_H
#define LUBAN_UTILS_H

#include "MurmurHash3.h"
#include "feature.pb.h"
#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>
#include <time.h>
#include <stdio.h>
#include <sys/time.h>

#define slot_shift 56
#define slot_mask 0xffffffffffffff
#define u_int64_t unsigned long long

namespace luban {
    u_int64_t mask(u_int64_t &&v, u_int64_t &slot);

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

#endif //LUBAN_UTILS_H