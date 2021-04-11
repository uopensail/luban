#ifndef LUBAN_BASIC_FPS_H
#define LUBAN_BASIC_FPS_H

#include <math.h>
#include "common.h"
#include "helper.h"
#include "utils.h"

namespace luban {
    static void kv(std::string &key, tensorflow::Feature &feature, ParamsHelper &params, std::vector<u_int64_t> &ret) {
        auto type = params.get<int>("type");
        auto slot = params.get<u_int64_t>("slot");
        std::vector<std::string> list;
        feature_to_strings(type, feature, list);
        for (auto &v:list) {
            ret.push_back(mask(mmh3(key + "|" + v), slot));
        }
    }

    static void raw(std::string &key, tensorflow::Feature &feature, ParamsHelper &params, std::vector<u_int64_t> &ret) {
        auto type = params.get<int>("type");
        auto slot = params.get<u_int64_t>("slot");
        std::vector<std::string> list;
        feature_to_strings(type, feature, list);
        for (auto &v:list) {
            ret.push_back(mask(mmh3(v), slot));
        }
    }

    static void bucket(std::string &key, tensorflow::Feature &feature, ParamsHelper &params,
                       std::vector<u_int64_t> &ret) {
        auto type = params.get<int>("type");
        auto slot = params.get<u_int64_t>("slot");
        float low = params.get<double>("low");
        float high = params.get<double>("high");
        int buckets = params.get<int>("buckets");
        std::vector<float> list;
        feature_to_floats(type, feature, list);
        for (auto &v:list) {
            ret.push_back(mask(mmh3(key + "|bucket|" + std::to_string(get_bucket(v, low, high, buckets))), slot));
        }
    }

    static void
    logint(std::string &key, tensorflow::Feature &feature, ParamsHelper &params, std::vector<u_int64_t> &ret) {
        auto type = params.get<int>("type");
        auto slot = (u_int64_t) params.get<int>("slot");
        std::vector<float> list;
        feature_to_floats(type, feature, list);
        for (auto &v:list) {
            ret.push_back(mask(mmh3(key + "|" + std::to_string(int(log(v)))), slot));
        }
        return;
    }

    static void merge(std::string &keyA, tensorflow::Feature &featureA,
                      std::string &keyB, tensorflow::Feature &featureB,
                      ParamsHelper &params, std::vector<u_int64_t> &ret) {
        auto typeA = params.get<int>("typeA");
        auto typeB = params.get<int>("typeB");
        auto slot = (u_int64_t) params.get<int>("slot");
        std::vector<std::string> listA, listB;
        feature_to_strings(typeA, featureA, listA);
        feature_to_strings(typeB, featureB, listB);
        for (auto &vA:listA) {
            for (auto &vB:listA) {
                ret.push_back(mask(mmh3(keyA + "|" + vA + "|" + keyB + "|" + vB), slot));
            }
        }
        return;
    }
}

#endif //LUBAN_BASIC_FPS_H
