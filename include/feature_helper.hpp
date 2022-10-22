#ifndef LUBAN_FEATURE_HELPER_HPP
#define LUBAN_FEATURE_HELPER_HPP

#pragma once

// feature.pb.h中的一些扩展的函数
#include <functional>

//判断类型
#define is_int(T) (std::is_same<T, long long>::value || std::is_same<T, int>::value || std::is_same<T, long>::value || std::is_same<T, unsigned long long>::value)
#define is_float(T) (std::is_same<T, float>::value || std::is_same<T, double>::value)
#define is_str(T) (std::is_same<T, std::string>::value)

//空的析构函数
static void delete_do_nothing(void *) {}

//通过key来查找某个feature
static SharedFeaturePtr get_feature_by_key(const SharedFeaturesPtr &features, const std::string &key)
{
    auto &feas_map = features->feature();
    auto iter = feas_map.find(key);
    if (iter == feas_map.end())
    {
        return nullptr;
    }

    //上层的features交给shared ptr托管, 所以具体特征就不设置析构函数
    return SharedFeaturePtr{(tensorflow::Feature *)(&(iter->second)), delete_do_nothing};
}

//将特征转化为数组
template <typename T>
static void to_array(const SharedFeaturePtr &feature, std::vector<T> &ret)
{
    if constexpr (is_int(T))
    {
        assert(feature->has_int64_list());
        for (int i = 0; i < feature->int64_list().value_size(); i++)
        {
            ret.push_back(static_cast<T>(feature->int64_list().value(i)));
        }
        return;
    }
    else if constexpr (is_float(T))
    {
        assert(feature->has_int64_list() || feature->has_float_list());
        //这里兼容, 会把整型转为浮点型
        if (feature->has_int64_list())
        {
            for (int i = 0; i < feature->int64_list().value_size(); i++)
            {
                ret.push_back(static_cast<T>(feature->int64_list().value(i)));
            }
            return;
        }
        else if (feature->has_float_list())
        {
            for (int i = 0; i < feature->float_list().value_size(); i++)
            {
                ret.push_back(static_cast<T>(feature->float_list().value(i)));
            }
            return;
        }
    }
    else if constexpr (is_str(T))
    {
        assert(feature->has_bytes_list());
        for (int i = 0; i < feature->bytes_list().value_size(); i++)
        {
            ret.push_back(feature->bytes_list().value(i));
        }
        return;
    }
    throw std::runtime_error("feature value error");
}

//将特征转化为标量, 取list中的第一个值
template <typename T>
static T to_scalar(const SharedFeaturePtr &feature)
{
    if constexpr (is_int(T))
    {
        assert(feature->has_int64_list());
        return static_cast<T>(feature->int64_list().value(0));
    }
    else if constexpr (is_float(T))
    {
        //这里兼容, 会把整型转为浮点型
        assert(feature->has_int64_list() || feature->has_float_list());
        if (feature->has_int64_list())
        {
            return static_cast<T>(feature->int64_list().value(0));
        }
        else
        {
            return static_cast<T>(feature->float_list().value(0));
        }
    }
    else if constexpr (is_str(T))
    {
        assert(feature->has_bytes_list());
        return feature->bytes_list().value(0);
    }
    throw std::runtime_error("feature value error");
}

//特征添加值
template <typename T>
static void add_value(const SharedFeaturePtr &feature, const T &v)
{
    if constexpr (is_int(T))
    {
        feature->mutable_int64_list()->add_value(static_cast<int64_t>(v));
        return;
    }
    else if constexpr (is_float(T))
    {
        feature->mutable_float_list()->add_value(static_cast<float>(v));
        return;
    }
    else if constexpr (is_str(T))
    {
        feature->mutable_bytes_list()->add_value(v);
        return;
    }
    throw std::runtime_error("value type error");
}

//处理feature, agg处理
template <typename U, typename V>
static SharedFeaturePtr agg_func(const SharedFeaturePtr &feature, std::function<U(const std::vector<V> &)> func)
{
    if constexpr (is_int(V) || is_float(V) || is_str(V))
    {
        SharedFeaturePtr ret(new tensorflow::Feature());
        std::vector<V> data;
        to_array<V>(feature, data);
        U value = func(data);
        add_value(ret, value);
        return ret;
    }
    return nullptr;
}

//处理feature, map处理
template <typename U, typename V>
static SharedFeaturePtr map_func(const SharedFeaturePtr &feature, std::function<U(V)> func)
{
    if constexpr (is_int(V) || is_float(V) || is_str(V))
    {
        SharedFeaturePtr ret(new tensorflow::Feature());
        std::vector<V> data;
        to_array<V>(feature, data);
        for (size_t i = 0; i < data.size(); i++)
        {
            auto tmp = func(data[i]);
            add_value<U>(ret, tmp);
        }
        return ret;
    }
    return nullptr;
}

//处理feature, 笛卡尔积的处理
template <typename U, typename V, typename W>
static SharedFeaturePtr cartesian_cross_func(const SharedFeaturePtr &featureA, const SharedFeaturePtr &featureB, std::function<U(V, W)> func)
{
    if constexpr ((is_int(V) || is_float(V) || is_str(V)) || (is_int(W) || is_float(W) || is_str(W)))
    {
        SharedFeaturePtr ret(new tensorflow::Feature());
        std::vector<V> dataA;
        std::vector<V> dataB;
        to_array<V>(featureA, dataA);
        to_array<W>(featureB, dataB);
        for (size_t i = 0; i < dataA.size(); i++)
        {
            for (size_t j = 0; j < dataB.size(); j++)
            {
                auto tmp = func(dataA[i], dataB[j]);
                add_value<U>(ret, tmp);
            }
        }
        return ret;
    }
    return nullptr;
}

//处理feature, 相同形状的值进行处理
template <typename U, typename V, typename W>
static SharedFeaturePtr hadamard_map_func(const SharedFeaturePtr &featureA, const SharedFeaturePtr &featureB, std::function<U(V, W)> func)
{
    if constexpr ((is_int(V) || is_float(V) || is_str(V)) || (is_int(W) || is_float(W) || is_str(W)))
    {
        SharedFeaturePtr ret(new tensorflow::Feature());
        std::vector<V> dataA;
        std::vector<V> dataB;
        to_array<V>(featureA, dataA);
        to_array<W>(featureB, dataB);
        size_t lenA = dataA.size();
        size_t lenB = dataA.size();

        if (lenA != lenB)
        {
            return nullptr;
        }
        for (size_t i = 0; i < lenA; i++)
        {
            auto tmp = func(dataA[i], dataB[i]);
            add_value<U>(ret, tmp);
        }
        return ret;
    }
    return nullptr;
}

//处理feature, 相同形状的值进行处理
template <typename U, typename V, typename W>
static SharedFeaturePtr hadamard_agg_func(const SharedFeaturePtr &featureA, const SharedFeaturePtr &featureB, std::function<U(const std::vector<V> &, const std::vector<W> &)> func)
{
    if constexpr ((is_int(V) || is_float(V) || is_str(V)) || (is_int(W) || is_float(W) || is_str(W)))
    {
        SharedFeaturePtr ret(new tensorflow::Feature());
        std::vector<V> dataA;
        std::vector<V> dataB;
        to_array<V>(featureA, dataA);
        to_array<W>(featureB, dataB);
        size_t lenA = dataA.size();
        size_t lenB = dataA.size();

        if (lenA != lenB)
        {
            return nullptr;
        }

        auto tmp = func(dataA, dataB);
        add_value<U>(ret, tmp);

        return ret;
    }
    return nullptr;
}

#endif // LUBAN_FEATURE_HELPER_HPP