#ifndef LUBAN_BASIC_FPS_H
#define LUBAN_BASIC_FPS_H

#include <math.h>
#include "common.h"
#include "helper.h"
#include "utils.h"

namespace luban {
    namespace box {
        std::vector<u_int64_t> &&kv(std::string &key, tensorflow::Feature &feature, ParamsHelper &params) {
            auto type = params.get<int>("type");
            auto slot = (u_int64_t) params.get<int>("slot");
            std::vector<std::string> list;
            feature_to_strings(type, feature, list);
            std::vector<u_int64_t> ret;
            for (auto &v:list) {
                ret.push_back(mask(mmh3(key + "|" + v), slot));
            }
            return std::move(ret);
        }

        std::vector<u_int64_t> &&logint(std::string &key, tensorflow::Feature &feature, ParamsHelper &params) {
            auto type = params.get<int>("type");
            auto slot = (u_int64_t) params.get<int>("slot");
            std::vector<float> list;
            feature_to_floats(type, feature, list);
            std::vector<u_int64_t> ret;
            for (auto &v:list) {
                ret.push_back(mask(mmh3(key + "|" + std::to_string(int(log(v)))), slot));
            }
            return std::move(ret);
        }

        std::vector<u_int64_t> &&merge(std::string &keyA, tensorflow::Feature &featureA,
                                       std::string &keyB, tensorflow::Feature &featureB,
                                       ParamsHelper &params) {
            auto typeA = params.get<int>("typeA");
            auto typeB = params.get<int>("typeB");
            auto slot = (u_int64_t) params.get<int>("slot");
            std::vector<std::string> listA, listB;
            feature_to_strings(typeA, featureA, listA);
            feature_to_strings(typeB, featureB, keyB);
            std::vector<u_int64_t> ret;
            for (auto &vA:listA) {
                for (auto &vB:listA) {
                    ret.push_back(mask(mmh3(keyA + "|" + vA + "|" + keyB + "|" + vB), slot));
                }
            }
            return std::move(ret);
        }

    } // namespace box
}

#endif //LUBAN_BASIC_FPS_H
