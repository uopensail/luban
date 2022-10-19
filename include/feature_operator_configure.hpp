#ifndef LUBAN_FEATURE_OPREATOR_CONFIGURE_HPP
#define LUBAN_FEATURE_OPREATOR_CONFIGURE_HPP

#pragma once

//特征处理算子的配置处理

#include "cpptoml.h"
#include "feature_helper.hpp"
#include <iostream>
#include <string>

/*
    VT_Constant_Int: 字面整型常量
    VT_Constant_Float: 字面浮点型常量
    VT_Constant_String: 字面字符串常量, 以引号作为字符串的开始和结束
    VT_Constant_IntList: 字面整型常量数组
    VT_Constant_FloatList: 字面浮点型常量数组
    VT_Constant_StringList: 字面字符串常量数组
    VT_RunTime_Int: 运行时的整型值
    VT_RunTime_Float: 运行时的浮点型值
    VT_RunTime_String: 运行时的字符串值
    VT_Original_Feature: 原始特征, tffeature类型的值, TensorFlow中定义
    VT_Named_Feature: 命名的变量, tffeature类型的值
    VT_Anonymous_Feature: 匿名变量, tffeature类型的值
*/
enum VariableType
{
    VT_Not_Defined = 0,
    VT_Constant_Int,
    VT_Constant_Float,
    VT_Constant_String,
    VT_Constant_IntList,
    VT_Constant_FloatList,
    VT_Constant_StringList,
    VT_RunTime_Int,
    VT_RunTime_Float,
    VT_RunTime_String,
    VT_Origin_Feature,
    VT_Named_Feature,
    VT_Anonymous_Feature
};

//函数的类型
enum FunctionType
{
    FT_Not_Defined = 0,
    FT_Single_Mapper,
    FT_Single_Aggregate,
    FT_Hadamard_Cross,
    FT_Hadamard_Mapper,
    FT_Hadamard_Aggregate
};

//配置中的参数
class ConfigureParameter
{
private:
    int index_;
    VariableType type_;
    SharedFeaturePtr data_;

public:
    ConfigureParameter() = delete;
    ConfigureParameter(int index, VariableType type, const SharedFeaturePtr &data) : index_(index),
                                                                                     type_(type), data_(data) {}
    ConfigureParameter(const ConfigureParameter &p) : index_(p.index_), type_(p.type_), data_(p.data_) {}

    //从toml配置中生成ConfigureParameter
    ConfigureParameter(const std::shared_ptr<cpptoml::table> &table)
    {
        assert(table->contains("index") && table->contains("type") && table->contains("data"));
        this->index_ = *(table->get_as<int>("index"));
        this->type_ = static_cast<VariableType>(*(table->get_as<int>("type")));
        this->data_ = SharedFeaturePtr(new tensorflow::Feature());

        switch (this->type_)
        {
        case VariableType::VT_Constant_Int:
            this->data_->mutable_int64_list()->add_value(*(table->get_as<int64_t>("data")));
            break;
        case VariableType::VT_Constant_IntList:
            for (const auto &v : *table->get_array_of<int64_t>("data"))
            {
                this->data_->mutable_int64_list()->add_value(v);
            }
            break;
        case VariableType::VT_Constant_Float:
            this->data_->mutable_float_list()->add_value(static_cast<float>(*(table->get_as<double>("data"))));
            break;
        case VariableType::VT_Constant_FloatList:
            for (const auto &v : *table->get_array_of<double>("data"))
            {
                this->data_->mutable_float_list()->add_value(static_cast<float>(v));
            }
            break;
        case VariableType::VT_Constant_StringList:
            for (const auto &v : *table->get_array_of<std::string>("data"))
            {
                this->data_->mutable_bytes_list()->add_value(v);
            }
            break;

        case VariableType::VT_RunTime_Float:
        case VariableType::VT_RunTime_Int:
        case VariableType::VT_RunTime_String:
        case VariableType::VT_Constant_String:
        case VariableType::VT_Origin_Feature:
        case VariableType::VT_Named_Feature:
        case VariableType::VT_Anonymous_Feature:
            this->data_->mutable_bytes_list()->add_value(*(table->get_as<std::string>("data")));
            break;
        default:
            break;
        }
    }

    ConfigureParameter &operator=(const ConfigureParameter &p)
    {
        if (this == &p)
        {
            return *this;
        }
        index_ = p.index_;
        type_ = p.type_;
        data_ = p.data_;
        return *this;
    }
    ~ConfigureParameter() {}
    const int &get_index() { return index_; }
    const VariableType &get_type() { return type_; }
    const SharedFeaturePtr &get_data() { return data_; }
};

//配置中的operator
class ConfigureOperator
{
private:
    VariableType type_;
    FunctionType ftype_;
    std::string name_;
    std::string function_;
    std::shared_ptr<std::vector<ConfigureParameter>> parameters_;

public:
    ConfigureOperator() = delete;
    ConfigureOperator(const ConfigureOperator &o) : type_(o.type_), ftype_(o.ftype_), name_(o.name_), function_(o.function_), parameters_(o.parameters_) {}
    ConfigureOperator &operator=(const ConfigureOperator &o)
    {
        if (this == &o)
        {
            return *this;
        }
        name_ = o.name_;
        function_ = o.function_;
        parameters_ = o.parameters_;
        type_ = o.type_;
        ftype_ = o.ftype_;
        return *this;
    }

    ConfigureOperator(const std::shared_ptr<cpptoml::table> &table)
    {
        assert(table->contains("name") && table->contains("func") && table->contains("params") && table->contains("type") && table->contains("func_type"));
        this->name_ = *(table->get_as<std::string>("name"));
        this->function_ = *(table->get_as<std::string>("func"));
        this->type_ = static_cast<VariableType>(*(table->get_as<int>("type")));
        this->ftype_ = static_cast<FunctionType>(*(table->get_as<int>("func_type")));
        this->parameters_ = std::shared_ptr<std::vector<ConfigureParameter>>(new std::vector<ConfigureParameter>());

        for (const auto &t : *table->get_table_array("params"))
        {
            this->parameters_->emplace_back(t);
        }
    }
    ~ConfigureOperator() {}
    const std::string &get_name() { return name_; }
    const std::string &get_function() { return function_; }
    const VariableType &get_type() { return type_; }
    const FunctionType &get_function_type() { return ftype_; }
    const std::shared_ptr<std::vector<ConfigureParameter>> &get_parameters() { return parameters_; }
};

#endif // LUBAN_FEATURE_OPREATOR_CONFIGURE_HPP