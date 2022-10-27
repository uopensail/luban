#ifndef LUBAN_BOX_HELPER_H
#define LUBAN_BOX_HELPER_H

#pragma once

#include "cpptoml.h"
#include "feature.pb.h"
#include <functional>
#include <iostream>
#include <typeinfo>
#include <vector>

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

class ParamsHelper
{
private:
    std::shared_ptr<cpptoml::table> param_table_ptr;

public:
    ParamsHelper() = delete;

    ~ParamsHelper() {}

    ParamsHelper(const std::shared_ptr<cpptoml::table> &table) : param_table_ptr(table) {}

    ParamsHelper(const ParamsHelper &param) : param_table_ptr(param.param_table_ptr) {}

    ParamsHelper &operator=(const ParamsHelper &param)
    {
        param_table_ptr = param.param_table_ptr;
        return *this;
    }

    template <typename T>
    T get(std::string key) const
    {
        if (param_table_ptr->contains(key))
        {
            return *(param_table_ptr->get_as<T>(key));
        }
        else
        {
            throw std::out_of_range{key + " is not a valid key"};
        }
    }

    template <typename T>
    T get(std::string key, T value) const
    {
        if (param_table_ptr->contains(key))
        {
            return *(param_table_ptr->get_as<T>(key));
        }
        else
        {
            return value;
        }
    }

    template <typename T>
    const std::vector<T> &get_array(const std::string &key)
    {
        if (param_table_ptr->contains(key))
        {
            return *(param_table_ptr->get_array_of<T>(key));
        }
        else
        {
            throw std::out_of_range{key + " is not a valid key"};
        }
    }
};

#endif // LUBAN_BOX_HELPER_H