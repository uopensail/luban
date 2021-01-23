#ifndef __LUBAN_BOX_UTILS_H__
#define __LUBAN_BOX_UTILS_H__

#include "MurmurHash3.h"
#include "feature.pb.h"
#include <dlfcn.h>
#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

#define slot_shift 56
#define slot_mask 0xffffffffffffff

#define same_type(x, T) constexpr(std::is_same<x, typename std::decay<T>::type>::value)

namespace luban
{

    namespace box
    {
        template <typename T>
        std::string &&to_string(T &v)
        {
            if (same_type(std::string, T))
            {
                return std::move(v);
            }
            else
            {
                return std::move(std::to_string(v));
            }
        }

        static u_int64_t mask(u_int64_t v, u_int64_t slot)
        {
            if (slot >= 0)
            {
                return (slot << slot_shift) + (slot_mask & v);
            }
            return v;
        }

        static u_int64_t mmh3(std::string &key)
        {
            u_int64_t ret[2];
            MurmurHash3_x64_128(key.c_str(), key.size(), 0, &ret);
            return ret[0];
        }

        template <typename T>
        std::tuple<void *, T *> load_externam_func(std::string &name, std::string &path)
        {
            void *handler = dlopen(path.c_str(), RTLD_LAZY);
            if (handler != nullptr)
            {
                std::cout << "load function: " << name << " from: " << path << " error: " << dlerror() << std::endl;
                return std::make_tuple(nullptr, nullptr);
            }

            T *func = (T *)dlsym(handler, name.c_str());
            if (dlerror() != nullptr)
            {
                std::cout << "load function: " << name << " from: " << path << " error: " << dlerror() << std::endl;
                dlclose(handler);
                return std::make_tuple(nullptr, nullptr);
            }
            if (func == nullptr)
            {
                dlclose(handler);
                return std::make_tuple(nullptr, nullptr);
            }
            return std::make_tuple(handler, func);
        }

        template <typename T>
        std::vector<T> &&fetch_all(tensorflow::Feature &feature)
        {
            std::vector<T> ret;
            if (same_type(int64_t, T))
            {
                auto vlist = feature.int64_list();
                for (size_t i = 0; i < vlist.value_size(); i++)
                {
                    ret.push_back(vlist.value(i));
                }
            }
            else if (same_type(std::string, T))
            {
                auto vlist = feature.bytes_list();
                for (size_t i = 0; i < vlist.value_size(); i++)
                {
                    ret.push_back(vlist.value(i));
                }
            }
            else if (same_type(float, T))
            {
                auto vlist = feature.float_list();
                for (size_t i = 0; i < vlist.value_size(); i++)
                {
                    ret.push_back(vlist.value(i));
                }
            }
            return std::move(ret);
        }
    } // namespace box
} // namespace luban

#endif //__LUBAN_BOX_UTILS_H__