#ifndef LUBAN_FEATURE_OPERATOR_HANDLER_HPP
#define LUBAN_FEATURE_OPERATOR_HANDLER_HPP

#pragma once
#include "feature_helper.hpp"
#include "feature_operator_configure.hpp"
#include "feature_operator_runtime.hpp"

//函数参数的辅助类
class FunctionParameterHelper
{
private:
    std::vector<RunTimeParameter> &params_;
    int index_;

public:
    FunctionParameterHelper() = delete;
    FunctionParameterHelper(std::vector<RunTimeParameter> &params) : params_(params), index_(0) {}
    ~FunctionParameterHelper() {}
    void *get()
    {
        return (void *)params_[index_++].get();
    }
};

//调用map算子函数
template <typename T, typename U>
static SharedFeaturePtr map_call(const SharedFeaturePtr &feature, T (*func)(U), std::vector<RunTimeParameter> &params)
{
    return map_func<T, U>(feature, func);
}

template <typename T, typename U, typename... ArgsType>
static SharedFeaturePtr map_call(const SharedFeaturePtr &feature, T (*func)(U, ArgsType...), std::vector<RunTimeParameter> &params)
{
    FunctionParameterHelper fp(params);
    auto myfunc = std::bind(func, std::placeholders::_1, *(ArgsType *)fp.get()...);
    return map_func<T, U>(feature, myfunc);
}

//调用agg算子函数
template <typename T, typename U>
static SharedFeaturePtr agg_call(const SharedFeaturePtr &feature, T (*func)(const std::vector<U> &), std::vector<RunTimeParameter> &params)
{
    return agg_func<T, U>(feature, func);
}

template <typename T, typename U, typename... ArgsType>
static SharedFeaturePtr agg_call(const SharedFeaturePtr &feature, T (*func)(const std::vector<U> &, ArgsType...), std::vector<RunTimeParameter> &params)
{
    FunctionParameterHelper fp(params);
    auto myfunc = std::bind(func, std::placeholders::_1, *(ArgsType *)fp.get()...);
    return agg_func<T, U>(feature, myfunc);
}

//调用cross算子函数
template <typename T, typename U, typename W>
static SharedFeaturePtr hadamard_cross_call(const SharedFeaturePtr &featureA, const SharedFeaturePtr &featureB, T (*func)(U, W), std::vector<RunTimeParameter> &params)
{
    return cross_func<T, U, W>(featureA, featureB, func);
}

template <typename T, typename U, typename W, typename... ArgsType>
static SharedFeaturePtr hadamard_cross_call(const SharedFeaturePtr &featureA, const SharedFeaturePtr &featureB, T (*func)(U, W, ArgsType...), std::vector<RunTimeParameter> &params)
{
    FunctionParameterHelper fp(params);
    auto myfunc = std::bind(func, std::placeholders::_1, std::placeholders::_2, *(ArgsType *)fp.get()...);
    return cross_func<T, U, W>(featureA, featureB, myfunc);
}

//调用hadamard_map算子函数
template <typename T, typename U, typename W>
static SharedFeaturePtr hadamard_map_call(const SharedFeaturePtr &featureA, const SharedFeaturePtr &featureB, T (*func)(U, W), std::vector<RunTimeParameter> &params)
{
    return hadamard_map_func<T, U, W>(featureA, featureB, func);
}

template <typename T, typename U, typename W, typename... ArgsType>
static SharedFeaturePtr hadamard_map_call(const SharedFeaturePtr &featureA, const SharedFeaturePtr &featureB, T (*func)(U, W, ArgsType...), std::vector<RunTimeParameter> &params)
{
    FunctionParameterHelper fp(params);
    auto myfunc = std::bind(func, std::placeholders::_1, std::placeholders::_2, *(ArgsType *)fp.get()...);
    return hadamard_map_func<T, U, W>(featureA, featureB, myfunc);
}

//调用hadamard_map算子函数
template <typename T, typename U, typename W>
static SharedFeaturePtr hadamard_agg_call(const SharedFeaturePtr &featureA, const SharedFeaturePtr &featureB, T (*func)(const std::vector<U> &, const std::vector<W> &), std::vector<RunTimeParameter> &params)
{
    return hadamard_agg_func<T, U, W>(featureA, featureB, func);
}

template <typename T, typename U, typename W, typename... ArgsType>
static SharedFeaturePtr hadamard_agg_call(const SharedFeaturePtr &featureA, const SharedFeaturePtr &featureB, T (*func)(const std::vector<U> &, const std::vector<W> &, ArgsType...), std::vector<RunTimeParameter> &params)
{
    FunctionParameterHelper fp(params);
    auto myfunc = std::bind(func, std::placeholders::_1, std::placeholders::_2, *(ArgsType *)fp.get()...);
    return hadamard_map_func<T, U, W>(featureA, featureB, myfunc);
}

//函数外面做一下封装, 为了统一放在map中个进行管理
#define map_function_wrapper(x)                                                                                              \
    static SharedFeaturePtr map_function_wrapper_##x(const SharedFeaturePtr &feature, std::vector<RunTimeParameter> &params) \
    {                                                                                                                        \
        return map_call(feature, x, params);                                                                                 \
    }

#define agg_function_wrapper(x)                                                                                              \
    static SharedFeaturePtr agg_function_wrapper_##x(const SharedFeaturePtr &feature, std::vector<RunTimeParameter> &params) \
    {                                                                                                                        \
        return agg_call(feature, x, params);                                                                                 \
    }

#define cross_function_wrapper(x)                                                                                                                                          \
    static SharedFeaturePtr hadamard_cross_function_wrapper_##x(const SharedFeaturePtr &featureA, const SharedFeaturePtr &featureB, std::vector<RunTimeParameter> &params) \
    {                                                                                                                                                                      \
        return hadamard_cross_call(featureA, featureB, x, params);                                                                                                         \
    }

#define hadamard_map_function_wrapper(x)                                                                                                                                 \
    static SharedFeaturePtr hadamard_map_function_wrapper_##x(const SharedFeaturePtr &featureA, const SharedFeaturePtr &featureB, std::vector<RunTimeParameter> &params) \
    {                                                                                                                                                                    \
        return hadamard_map_call(featureA, featureB, x, params);                                                                                                         \
    }

#define hadamard_agg_function_wrapper(x)                                                                                                                                 \
    static SharedFeaturePtr hadamard_agg_function_wrapper_##x(const SharedFeaturePtr &featureA, const SharedFeaturePtr &featureB, std::vector<RunTimeParameter> &params) \
    {                                                                                                                                                                    \
        return hadamard_agg_call(featureA, featureB, x, params);                                                                                                         \
    }

//把封装的函数添加到全局map中
#define add_map_func_to_global_oprs(global_oprs, x) \
    {                                               \
        global_oprs[#x] = map_function_wrapper_##x; \
    }

#define add_agg_func_to_global_oprs(global_oprs, x) \
    {                                               \
        global_oprs[#x] = agg_function_wrapper_##x; \
    }
#define add_hadamard_cross_func_to_global_oprs(global_oprs, x) \
    {                                                          \
        global_oprs[#x] = hadamard_cross_function_wrapper_##x; \
    }
#define add_hadamard_map_func_to_global_oprs(global_oprs, x) \
    {                                                        \
        global_oprs[#x] = hadamard_map_function_wrapper_##x; \
    }

#define add_hadamard_agg_func_to_global_oprs(global_oprs, x) \
    {                                                        \
        global_oprs[#x] = hadamard_agg_function_wrapper_##x; \
    }
#endif // LUBAN_FEATURE_OPERATOR_HANDLER_HPP