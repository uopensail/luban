#ifndef LUBAN_OPR_HPP
#define LUBAN_OPR_HPP

#pragma once

#include "feature_operator_handler.hpp"
#include "feature_operators.h"
#include <unordered_map>
#include "feature_operators.h"
//定义各类函数的类型
#define SingleMapFunction std::function<SharedFeaturePtr(const SharedFeaturePtr &feature, std::vector<RunTimeParameter> &)>
#define SingleAggFunction std::function<SharedFeaturePtr(const SharedFeaturePtr &feature, std::vector<RunTimeParameter> &)>
#define CartesianCrossFunction std::function<SharedFeaturePtr(const SharedFeaturePtr &featureA, const SharedFeaturePtr &featureB, std::vector<RunTimeParameter> &)>
#define HadamardMapFunction std::function<SharedFeaturePtr(const SharedFeaturePtr &featureA, const SharedFeaturePtr &featureB, std::vector<RunTimeParameter> &)>
#define HadamardAggFunction std::function<SharedFeaturePtr(const SharedFeaturePtr &featureA, const SharedFeaturePtr &featureB, std::vector<RunTimeParameter> &)>

class FeatureOperatorToolkit
{
private:
    std::unordered_map<std::string, SingleMapFunction> map_oprs_;
    std::unordered_map<std::string, SingleAggFunction> agg_oprs_;
    std::unordered_map<std::string, CartesianCrossFunction> cartesian_cross_oprs_;
    std::unordered_map<std::string, HadamardMapFunction> hadamard_map_oprs_;
    std::unordered_map<std::string, HadamardAggFunction> hadamard_agg_oprs_;

private:
    //获得特征
    SharedFeaturePtr get(ConfigureParameter &p, RunTimeFeatures &features)
    {
        auto &data = p.get_data();
        auto key = to_scalar<std::string>(data);
        auto &type = p.get_type();
        switch (type)
        {
        case VariableType::VT_Origin_Feature:
            return get_feature_by_key(features.get_origin(), key);
        case VariableType::VT_Selected_Feature:
            return get_feature_by_key(features.get_selected(), key);
        case VariableType::VT_Anonymous_Feature:
            return get_feature_by_key(features.get_anonymous(), key);
        default:
            return nullptr;
        }
    }

    void call_map_func(ConfigureOperator &o, RunTimeFeatures &features)
    {
        auto &cfg_params = o.get_parameters();
        const std::string &func = o.get_function();
        const std::string &name = o.get_name();
        VariableType type = o.get_type();
        auto iter = this->map_oprs_.find(func);
        if (iter == this->map_oprs_.end())
        {
            return;
        }
        //获得特征
        SharedFeaturePtr feature = this->get(cfg_params->at(0));
        if (feature == nullptr)
        {
            return;
        }
        std::cout << feature->DebugString() << std::endl;

        //生成参数
        std::vector<RunTimeParameter> parameters;
        for (size_t i = 1; i < cfg_params->size(); i++)
        {
            parameters.push_back({cfg_params->at(i)});
        }
        auto tmp = iter->second(feature, parameters);
        if (tmp != nullptr)
        {
            features.add_value(type, name, tmp);
        }
    }

    void call_agg_func(ConfigureOperator &o, RunTimeFeatures &features)
    {
        auto &cfg_params = o.get_parameters();
        const std::string &func = o.get_function();
        const std::string &name = o.get_name();
        VariableType type = o.get_type();
        auto iter = this->agg_oprs_.find(func);
        if (iter == this->agg_oprs_.end())
        {
            return;
        }
        //获得特征
        SharedFeaturePtr feature = this->get(cfg_params->at(0));
        if (feature == nullptr)
        {
            return;
        }
        //生成参数
        std::vector<RunTimeParameter> parameters;
        for (size_t i = 1; i < cfg_params->size(); i++)
        {
            parameters.push_back({cfg_params->at(i)});
        }
        auto tmp = iter->second(feature, parameters);
        if (tmp != nullptr)
        {
            features.add_value(type, name, tmp);
        }
    }

    void call_cartesian_cross_func(ConfigureOperator &o, RunTimeFeatures &features)
    {
        auto &cfg_params = o.get_parameters();
        const std::string &func = o.get_function();
        const std::string &name = o.get_name();
        VariableType type = o.get_type();
        auto iter = this->cartesian_cross_oprs_.find(func);
        if (iter == this->cartesian_cross_oprs_.end())
        {
            return;
        }
        //获得特征
        SharedFeaturePtr feature_1 = this->get(cfg_params->at(0));
        SharedFeaturePtr feature_2 = this->get(cfg_params->at(1));
        if (feature_1 == nullptr || feature_2 == nullptr)
        {
            return;
        }
        //生成参数
        std::vector<RunTimeParameter> parameters;
        for (size_t i = 2; i < cfg_params->size(); i++)
        {
            parameters.push_back({cfg_params->at(i)});
        }
        auto tmp = iter->second(feature_1, feature_2, parameters);
        if (tmp != nullptr)
        {
            features.add_value(type, name, tmp);
        }
    }

    void call_hadamard_map_func(ConfigureOperator &o, RunTimeFeatures &features)
    {
        auto &cfg_params = o.get_parameters();
        const std::string &func = o.get_function();
        const std::string &name = o.get_name();
        VariableType type = o.get_type();
        auto iter = this->hadamard_map_oprs_.find(func);
        if (iter == this->hadamard_map_oprs_.end())
        {
            return;
        }
        //获得特征
        SharedFeaturePtr feature_1 = this->get(cfg_params->at(0));
        SharedFeaturePtr feature_2 = this->get(cfg_params->at(1));
        if (feature_1 == nullptr || feature_2 == nullptr)
        {
            return;
        }
        //生成参数
        std::vector<RunTimeParameter> parameters;
        for (size_t i = 2; i < cfg_params->size(); i++)
        {
            parameters.push_back({cfg_params->at(i), features});
        }
        auto tmp = iter->second(feature_1, feature_2, parameters);
        if (tmp != nullptr)
        {
            features.add_value(type, name, tmp);
        }
    }

    void call_hadamard_agg_func(ConfigureOperator &o, RunTimeFeatures &features)
    {
        auto &cfg_params = o.get_parameters();
        const std::string &func = o.get_function();
        const std::string &name = o.get_name();
        VariableType type = o.get_type();
        auto iter = this->hadamard_agg_oprs_.find(func);
        if (iter == this->hadamard_agg_oprs_.end())
        {
            return;
        }
        //获得特征
        SharedFeaturePtr feature_1 = this->get(cfg_params->at(0));
        SharedFeaturePtr feature_2 = this->get(cfg_params->at(1));
        if (feature_1 == nullptr || feature_2 == nullptr)
        {
            return;
        }

        //生成参数
        std::vector<RunTimeParameter> parameters;
        for (size_t i = 2; i < cfg_params->size(); i++)
        {
            parameters.push_back({cfg_params->at(i), features});
        }
        auto tmp = iter->second(feature_1, feature_2, parameters);
        if (tmp != nullptr)
        {
            features.add_value(type, name, tmp);
        }
    }

public:
    FeatureOperatorToolkit()
    {
        add_map_func_to_global_oprs(this->map_oprs_, s_map_add);
    }

    ~FeatureOperatorToolkit() {}
    void call(ConfigureOperator &o, RunTimeFeatures &features)
    {
        switch (o.get_function_type())
        {
        case FunctionType::FT_Single_Mapper:
            this->call_map_func(o, features);
            return;
        case FunctionType::FT_Single_Aggregate:
            this->call_agg_func(o, features);
            return;
        case FunctionType::FT_Cartesian_Cross:
            this->call_cartesian_cross_func(o, features);
            return;
        case FunctionType::FT_Hadamard_Mapper:
            this->call_hadamard_map_func(o, features);
            return;
        case FunctionType::FT_Hadamard_Aggregate:
            this->call_hadamard_agg_func(o, features);
            return;
        default:
            return;
        }
    }
};

#endif // LUBAN_OPR_HPP