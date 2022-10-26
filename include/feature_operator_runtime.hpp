#ifndef LUBAN_FEATURE_OPERATOR_RUNTIME_HPP
#define LUBAN_FEATURE_OPERATOR_RUNTIME_HPP

#pragma once
#include "feature_helper.hpp"
#include "feature_operator_configure.hpp"
#include <chrono>

//运行时的特征
class RunTimeFeatures
{
private:
    SharedFeaturesPtr origin_;
    SharedFeaturesPtr selected_;
    SharedFeaturesPtr anonymous_;

public:
    RunTimeFeatures() = delete;
    RunTimeFeatures(const RunTimeFeatures &features) : origin_(features.origin_), selected_(features.selected_), anonymous_(features.anonymous_) {}
    RunTimeFeatures &operator=(const RunTimeFeatures &features)
    {
        if (this == &features)
        {
            return *this;
        }
        this->origin_ = features.origin_;
        this->selected_ = features.selected_;
        this->anonymous_ = features.anonymous_;
        return *this;
    }
    RunTimeFeatures(const SharedFeaturesPtr &features) : origin_(features),
                                                         selected_(new tensorflow::Features{}),
                                                         anonymous_(new tensorflow::Features{})

    {
    }
    ~RunTimeFeatures() {}
    const SharedFeaturesPtr &get_origin() { return this->origin_; }
    const SharedFeaturesPtr &get_selected() { return this->selected_; }
    const SharedFeaturesPtr &get_anonymous() { return this->anonymous_; }

    //添加一个值
    void add_value(VariableType type, const std::string &key, const SharedFeaturePtr &feature)
    {
        assert(type == VariableType::VT_Anonymous_Feature || type == VariableType::VT_Selected_Feature);
        switch (type)
        {
        case VariableType::VT_Anonymous_Feature:
            (*this->anonymous_->mutable_feature())[key] = *feature;
            return;
        case VariableType::VT_Selected_Feature:
            (*this->selected_->mutable_feature())[key] = *feature;
            return;
        default:
            return;
        }
    }

    //取某个特征
    SharedFeaturePtr get(VariableType type, const std::string &key)
    {
        assert(type == VariableType::VT_Anonymous_Feature || type == VariableType::VT_Selected_Feature || type == VariableType::VT_Origin_Feature);
        switch (type)
        {
        case VariableType::VT_Anonymous_Feature:
            return get_feature_by_key(this->anonymous_, key);
        case VariableType::VT_Selected_Feature:
            return get_feature_by_key(this->selected_, key);
        case VariableType::VT_Origin_Feature:
            return get_feature_by_key(this->origin_, key);
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

public:
    RunTimeParameter() = delete;
    RunTimeParameter(ConfigureParameter &p) : type_(p.get_type())
    {
        std::string name;
        const SharedFeaturePtr &data = p.get_data();
        switch (this->type_)
        {
        case VariableType::VT_Int:
            this->int_ = to_scalar<int64_t>(data);
            break;
        case VariableType::VT_Float:
            this->float_ = to_scalar<float>(data);
            break;
        case VariableType::VT_String:
            this->str_ = to_scalar<std::string>(data);
            break;
        case VariableType::VT_IntList:
            to_array<int64_t>(data, this->int_list_);
            break;
        case VariableType::VT_FloatList:
            to_array<float>(data, this->float_list_);
            break;
        case VariableType::VT_StringList:
            to_array<std::string>(data, this->str_list_);
            break;
        default:
            break;
        }
    }

    ~RunTimeParameter() {}

    void *get()
    {
        switch (type_)
        {
        case VariableType::VT_Int:
            return &this->int_;
        case VariableType::VT_Float:
            return &this->float_;
        case VariableType::VT_String:
            return &this->str_;
        case VariableType::VT_IntList:
            return &this->int_list_;
        case VariableType::VT_FloatList:
            return &this->float_list_;
        case VariableType::VT_StringList:
            return &this->str_list_;
        default:
            return nullptr;
        }
    }
};
#endif // LUBAN_FEATURE_OPERATOR_RUNTIME_HPP