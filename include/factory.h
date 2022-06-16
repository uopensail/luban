#ifndef LUBAN_FACTORY_H__
#define LUBAN_FACTORY_H__

#include "basic_fps.h"
#include "common.h"
#include <unordered_map>

namespace luban
{
    class Factory
    {
    private:
        std::unordered_map<std::string, single_function> single_funcs;
        std::unordered_map<std::string, cross_function> cross_funcs;

    public:
        Factory()
        {
            single_funcs["kv"] = luban::kv;
            single_funcs["log_int"] = luban::log_int;
            single_funcs["raw"] = luban::raw;
            single_funcs["bucket"] = luban::bucket;
            single_funcs["hour_diff"] = luban::hour_diff;
            single_funcs["log_bucket"] = luban::log_bucket;
            cross_funcs["co_occur"] = luban::co_occur;
            cross_funcs["merge"] = luban::merge;
        }

        Factory(const Factory &) = delete;

        Factory &operator=(const Factory &) = delete;

        ~Factory()
        {
            single_funcs.clear();
            cross_funcs.clear();
        }

        void single_process(std::string &func, std::string &key, tensorflow::Feature &feature, ParamsHelper &params,
                            std::vector<u_int64_t> &ret)
        {
            auto iter = single_funcs.find(func);
            if (iter == single_funcs.end())
            {
                return;
            }
            auto foo = iter->second;
            foo(key, feature, params, ret);
        }

        void cross_process(std::string &func, std::string &keyA, tensorflow::Feature &featureA, std::string &keyB,
                           tensorflow::Feature &featureB, ParamsHelper &params, std::vector<u_int64_t> &ret)
        {
            auto iter = cross_funcs.find(func);
            if (iter == cross_funcs.end())
            {
                return;
            }
            auto foo = iter->second;
            foo(keyA, featureA, keyB, featureB, params, ret);
        }
    };
} // namespace luban
#endif // LUBAN_FACTORY_H__