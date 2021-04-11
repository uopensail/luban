#ifndef LUBAN_FACTOR_H__
#define LUBAN_FACTOR_H__

#include "common.h"
#include "basic_fps.h"
#include <unordered_map>

namespace luban {
    class Factory {
    private:
        std::unordered_map<std::string, single_function> single_funcs;
        std::unordered_map<std::string, cross_function> cross_funcs;

    public:
        Factory() {
            single_funcs["kv"] = kv;
            single_funcs["logint"] = logint;
            single_funcs["raw"] = raw;
            single_funcs["bucket"] = bucket;
            cross_funcs["merge"] = merge;
        }

        Factory(const Factory &) = delete;

        Factory &operator=(const Factory &) = delete;

        ~Factory() {
            single_funcs.clear();
            cross_funcs.clear();
        }

        void single_process(std::string &func, std::string &key, tensorflow::Feature &feature, ParamsHelper &params,
                            std::vector<u_int64_t> &ret) {
            auto iter = single_funcs.find(func);
            if (iter == single_funcs.end()) {
                return;
            }
            auto foo = iter->second;
            foo(key, feature, params, ret);
        }

        void cross_process(std::string &func, std::string &keyA, tensorflow::Feature &featureA, std::string &keyB,
                           tensorflow::Feature &featureB, ParamsHelper &params, std::vector<u_int64_t> &ret) {
            auto iter = cross_funcs.find(func);
            if (iter == cross_funcs.end()) {
                return;
            }
            auto foo = iter->second;
            foo(keyA, featureA, keyB, featureB, params, ret);
        }
    };
} // namespace luban
#endif //LUBAN_FACTOR_H__