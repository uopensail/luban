#ifndef LUBAN_FEATURE_OPERATOR_RUNTIME_HPP
#define LUBAN_FEATURE_OPERATOR_RUNTIME_HPP

#pragma once
#include "feature_helper.hpp"
#include "feature_operator_configure.hpp"
#include <chrono>

const std::string RT_TimeStampMs = "__RT_I_TIMESTAMP_MS__";
const std::string RT_TimeStampS = "__RT_I_TIMESTAMP_S__";

//获得时间戳, 毫秒
static int64_t get_timestamp_millisecond()
{
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    return now.count();
}

//获得时间戳, 秒
static int64_t get_timestamp_second()
{
    return get_timestamp_millisecond() / 1000;
}

//获得运行时整型特征值
static int64_t get_runtime_integer_variable(const std::string &var_name)
{
    if (var_name == RT_TimeStampMs)
    {
        return get_timestamp_millisecond();
    }
    else if (var_name == RT_TimeStampS)
    {
        return get_timestamp_second();
    }

    return 0;
}

//运行时的特征
class RunTimeFeatures
{
private:
    SharedFeaturesPtr origin_;
    SharedFeaturesPtr named_;
    SharedFeaturesPtr anonymous_;

public:
    RunTimeFeatures() = delete;
    RunTimeFeatures(const RunTimeFeatures &features) : origin_(features.origin_), named_(features.named_), anonymous_(features.anonymous_) {}
    RunTimeFeatures &operator=(const RunTimeFeatures &features)
    {
        if (this == &features)
        {
            return *this;
        }
        origin_ = features.origin_;
        named_ = features.named_;
        anonymous_ = features.anonymous_;
        return *this;
    }
    RunTimeFeatures(const SharedFeaturesPtr &features) : origin_(features),
                                                         named_(new tensorflow::Features{}),
                                                         anonymous_(new tensorflow::Features{})

    {
    }
    ~RunTimeFeatures() {}
    const SharedFeaturesPtr &get_origin() { return origin_; }
    const SharedFeaturesPtr &get_named() { return named_; }
    const SharedFeaturesPtr &get_anonymous() { return anonymous_; }

    //添加一个值
    void add_value(VariableType type, const std::string &key, const SharedFeaturePtr &feature)
    {
        assert(type == VariableType::VT_Anonymous_Feature || type == VariableType::VT_Named_Feature);
        switch (type)
        {
        case VariableType::VT_Anonymous_Feature:
            (*anonymous_->mutable_feature())[key] = *feature;
            return;
        case VariableType::VT_Named_Feature:
            (*named_->mutable_feature())[key] = *feature;
            return;
        default:
            return;
        }
    }

    //取某个特征
    SharedFeaturePtr get(VariableType type, const std::string &key)
    {
        assert(type == VariableType::VT_Anonymous_Feature || type == VariableType::VT_Named_Feature || type == VariableType::VT_Origin_Feature);
        switch (type)
        {
        case VariableType::VT_Anonymous_Feature:
            return get_feature_by_key(anonymous_, key);
        case VariableType::VT_Named_Feature:
            return get_feature_by_key(named_, key);
        case VariableType::VT_Origin_Feature:
            return get_feature_by_key(origin_, key);
        default:
            return nullptr;
        }
    }
};

//定义参数的配置结构, 数据的类型和函数的参数类型是完全匹配的
class RunTimeParameter
{
private:
    VariableType type_;
    int64_t int_;
    float float_;
    std::string str_;
    std::vector<int64_t> int_list_;
    std::vector<float> float_list_;
    std::vector<std::string> str_list_;
    SharedFeaturePtr feature_;

public:
    RunTimeParameter() = delete;
    RunTimeParameter(ConfigureParameter &p, RunTimeFeatures &features) : type_(p.get_type())
    {
        std::string name;
        const SharedFeaturePtr &data = p.get_data();
        switch (this->type_)
        {
        case VariableType::VT_Constant_Int:
            this->int_ = to_scalar<int64_t>(data);
            break;
        case VariableType::VT_Constant_Float:
            this->float_ = to_scalar<float>(data);
            break;
        case VariableType::VT_Constant_String:
            this->str_ = to_scalar<std::string>(data);
            break;
        case VariableType::VT_Constant_IntList:
            to_array<int64_t>(data, this->int_list_);
            break;
        case VariableType::VT_Constant_FloatList:
            to_array<float>(data, this->float_list_);
            break;
        case VariableType::VT_Constant_StringList:
            to_array<std::string>(data, this->str_list_);
            break;
        case VariableType::VT_RunTime_Int:
            name = to_scalar<std::string>(data);
            int_ = get_runtime_integer_variable(name);
            break;
        case VariableType::VT_RunTime_Float:
        case VariableType::VT_RunTime_String:
            break;
        case VariableType::VT_Anonymous_Feature:
        case VariableType::VT_Named_Feature:
        case VariableType::VT_Origin_Feature:
            name = to_scalar<std::string>(p.get_data());
            feature_ = features.get(type_, name);
            break;
        default:
            break;
        }
    }

    ~RunTimeParameter() {}

    SharedFeaturePtr get_feature() { return feature_; }

    void *get()
    {
        switch (type_)
        {
        case VariableType::VT_Constant_Int:
        case VariableType::VT_RunTime_Int:
            return &int_;
        case VariableType::VT_Constant_Float:
        case VariableType::VT_RunTime_Float:
            return &float_;
        case VariableType::VT_Constant_String:
        case VariableType::VT_RunTime_String:
            return &str_;
        case VariableType::VT_Constant_IntList:
            return &int_list_;
        case VariableType::VT_Constant_FloatList:
            return &float_list_;
        case VariableType::VT_Constant_StringList:
            return &str_list_;
        default:
            return nullptr;
        }
    }
};
#endif // LUBAN_FEATURE_OPERATOR_RUNTIME_HPP