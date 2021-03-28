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

    private:
        std::vector<void *> handlers;

    public:
        Factory() {
            single_funcs["kv"] = kv;
            cross_funcs["merge"] = merge;
        }

        Factory(const Factory &) = delete;

        Factory &operator=(const Factory &) = delete;

        ~Factory() {
            for (size_t i = 0; i < handlers.size(); i++) {
                dlclose(handlers[i]);
                handlers[i] = nullptr;
            }
            handlers.clear();
            single_funcs.clear();
            cross_funcs.clear();
        }

        std::vector<u_int64_t> &&
        single_process(std::string &func, std::string &key, tensorflow::Feature &feature, ParamsHelper &params) {
            auto iter = single_funcs.find(func);
            if (iter == single_funcs.end()) {
                std::vector<u_int64_t> ret;
                return std::move(ret);
            }
            auto foo = iter->second;
            return foo(key, feature, params);
        }

        std::vector<u_int64_t> &&
        cross_process(std::string &func, std::string &keyA, tensorflow::Feature &featureA, std::string &keyB,
                      tensorflow::Feature &featureB, ParamsHelper &params) {
            auto iter = cross_funcs.find(func);
            if (iter == cross_funcs.end()) {
                std::vector<u_int64_t> ret;
                return std::move(ret);
            }
            auto foo = iter->second;
            return foo(keyA, featureA, keyB, featureB, params);
        }

        bool load(ParamsHelper &params) {
            auto name = params.get<std::string>("func_name");
            auto path = params.get<std::string>("so_path");
            auto type = params.get<std::string>("type");
            if (type == "single") {
                auto tmp_tuple = load_externam_func<single_function>(name, path);
                void *handler = std::get<0>(tmp_tuple);
                single_function func = std::get<1>(tmp_tuple);
                if (handler == nullptr || func == nullptr) {
                    return false;
                }
                handlers.push_back(handler);
                single_funcs[name] = func;
                return true;
            } else if (type == "cross") {
                auto tmp_tuple = load_externam_func<cross_function>(name, path);
                void *handler = std::get<0>(tmp_tuple);
                cross_function func = std::get<1>(tmp_tuple);
                if (handler == nullptr || func == nullptr) {
                    return false;
                }
                handlers.push_back(handler);
                cross_funcs[name] = func;
                return true;
            }
        }
    };
} // namespace luban
#endif //LUBAN_FACTOR_H__