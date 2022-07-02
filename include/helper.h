#ifndef __LUBAN_BOX_HELPER_H__
#define __LUBAN_BOX_HELPER_H__

#include "cpptoml.h"
#include "feature.pb.h"
#include <vector>
#include <iostream>

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
    std::vector<T> get_array(const std::string &key) const
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

// tensorflow::Feature取值
template <typename T>
static T get_value(const tensorflow::Feature &feature, int index)
{
    if constexpr (std::is_same_v<T, std::string>)
    {
        assert(feature.has_bytes_list());
        assert(index >= 0 && index < feature.bytes_list().value_size());
        return feature.bytes_list().value(index);
    }
    else if constexpr (std::is_same_v<T, float>)
    {
        assert(feature.has_float_list());
        assert(index >= 0 && index < feature.float_list().value_size());
        return feature.float_list().value(index);
    }
    else if constexpr (std::is_same_v<T, int64_t>)
    {
        assert(feature.has_int64_list());
        assert(index >= 0 && index < feature.int64_list().value_size());
        return feature.int64_list().value(index);
    }
}

//获得浮点数的值
static float get_float(const tensorflow::Feature &feature, int index)
{
    switch (feature.kind_case())
    {
    case tensorflow::Feature::KindCase::kFloatList:
        return get_value<float>(feature, index);
    case tensorflow::Feature::KindCase::kInt64List:
        return static_cast<float>(get_value<int64_t>(feature, index));
    case tensorflow::Feature::KindCase::kBytesList:
        return std::stof(get_value<std::string>(feature, index));
    default:
        throw std::invalid_argument{"value can not cast to float"};
    }
}

//获得字符串的值
static std::string get_string(const tensorflow::Feature &feature, int index)
{
    switch (feature.kind_case())
    {
    case tensorflow::Feature::KindCase::kFloatList:
        return std::to_string(get_value<float>(feature, index));
    case tensorflow::Feature::KindCase::kInt64List:
        return std::to_string(get_value<int64_t>(feature, index));
    case tensorflow::Feature::KindCase::kBytesList:
        return get_value<std::string>(feature, index);
    default:
        throw std::invalid_argument{"value can not cast to string"};
    }
}

//获得长度
static int value_size(const tensorflow::Feature &feature)
{
    switch (feature.kind_case())
    {
    case tensorflow::Feature::KindCase::kBytesList:
        return feature.bytes_list().value_size();
    case tensorflow::Feature::KindCase::kFloatList:
        return feature.float_list().value_size();
    case tensorflow::Feature::KindCase::kInt64List:
        return feature.int64_list().value_size();
    default:
        return 0;
    }
}

#endif //__LUBAN_BOX_HELPER_H__