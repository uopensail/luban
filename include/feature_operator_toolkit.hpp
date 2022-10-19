#ifndef LUBAN_OPR_HPP
#define LUBAN_OPR_HPP

#pragma once

#include "feature_operator_handler.hpp"
#include "feature_operators.h"
#include <unordered_map>
class FeatureOperatorToolkit
{
private:
    std::unordered_map<std::string, std::function<SharedFeaturePtr(const SharedFeaturePtr &feature, std::vector<RunTimeParameter> &)>> map_oprs_;
    std::unordered_map<std::string, std::function<SharedFeaturePtr(const SharedFeaturePtr &feature, std::vector<RunTimeParameter> &)>> agg_oprs_;
    std::unordered_map<std::string, std::function<SharedFeaturePtr(const SharedFeaturePtr &featureA, const SharedFeaturePtr &featureB, std::vector<RunTimeParameter> &)>> hadamard_cross_oprs_;
    std::unordered_map<std::string, std::function<SharedFeaturePtr(const SharedFeaturePtr &featureA, const SharedFeaturePtr &featureB, std::vector<RunTimeParameter> &)>> hadamard_map_oprs_;
    std::unordered_map<std::string, std::function<SharedFeaturePtr(const SharedFeaturePtr &featureA, const SharedFeaturePtr &featureB, std::vector<RunTimeParameter> &)>> hadamard_agg_oprs_;

public:
    void call_map_func(ConfigureOperator &o, RunTimeFeatures &features)
    {
        auto &cfg_params = o.get_parameters();
        const std::string &func = o.get_function();
        const std::string &name = o.get_name();
        VariableType type = o.get_type();
        auto iter = map_oprs_.find(func);
        if (iter == map_oprs_.end())
        {
            return;
        }
        //获得特征
        RunTimeParameter rt_feature(cfg_params->at(0), features);
        SharedFeaturePtr feature = rt_feature.get_feature();
        std::cout<<feature->DebugString()<<std::endl;

        //生成参数
        std::vector<RunTimeParameter> parameters;
        for (size_t i = 1; i < cfg_params->size(); i++)
        {
            parameters.push_back({cfg_params->at(i), features});
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
        auto iter = agg_oprs_.find(func);
        if (iter == agg_oprs_.end())
        {
            return;
        }
        //获得特征
        RunTimeParameter rt_feature(cfg_params->at(0), features);
        SharedFeaturePtr feature = rt_feature.get_feature();

        //生成参数
        std::vector<RunTimeParameter> parameters;
        for (size_t i = 1; i < cfg_params->size(); i++)
        {
            parameters.push_back({cfg_params->at(i), features});
        }
        auto tmp = iter->second(feature, parameters);
        if (tmp != nullptr)
        {
            features.add_value(type, name, tmp);
        }
    }

    void call_hadamard_cross_func(ConfigureOperator &o, RunTimeFeatures &features)
    {
        auto &cfg_params = o.get_parameters();
        const std::string &func = o.get_function();
        const std::string &name = o.get_name();
        VariableType type = o.get_type();
        auto iter = hadamard_cross_oprs_.find(func);
        if (iter == hadamard_cross_oprs_.end())
        {
            return;
        }
        //获得特征
        RunTimeParameter rt_feature_1(cfg_params->at(0), features);
        RunTimeParameter rt_feature_2(cfg_params->at(1), features);
        SharedFeaturePtr feature_1 = rt_feature_1.get_feature();
        SharedFeaturePtr feature_2 = rt_feature_2.get_feature();

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

    void call_hadamard_map_func(ConfigureOperator &o, RunTimeFeatures &features)
    {
        auto &cfg_params = o.get_parameters();
        const std::string &func = o.get_function();
        const std::string &name = o.get_name();
        VariableType type = o.get_type();
        auto iter = hadamard_map_oprs_.find(func);
        if (iter == hadamard_map_oprs_.end())
        {
            return;
        }
        //获得特征
        RunTimeParameter rt_feature_1(cfg_params->at(0), features);
        RunTimeParameter rt_feature_2(cfg_params->at(1), features);
        SharedFeaturePtr feature_1 = rt_feature_1.get_feature();
        SharedFeaturePtr feature_2 = rt_feature_2.get_feature();

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
        auto iter = hadamard_agg_oprs_.find(func);
        if (iter == hadamard_agg_oprs_.end())
        {
            return;
        }
        //获得特征
        RunTimeParameter rt_feature_1(cfg_params->at(0), features);
        RunTimeParameter rt_feature_2(cfg_params->at(1), features);
        SharedFeaturePtr feature_1 = rt_feature_1.get_feature();
        SharedFeaturePtr feature_2 = rt_feature_2.get_feature();

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
        add_map_func_to_global_oprs(map_oprs_, s_map_add);
    }

    ~FeatureOperatorToolkit() {}
    void call(ConfigureOperator &o, RunTimeFeatures &features)
    {
        switch (o.get_function_type())
        {
        case FunctionType::FT_Single_Mapper:
            call_map_func(o, features);
            return;
        case FunctionType::FT_Single_Aggregate:
            call_agg_func(o, features);
            return;
        case FunctionType::FT_Hadamard_Cross:
            call_hadamard_cross_func(o, features);
            return;
        case FunctionType::FT_Hadamard_Mapper:
            call_hadamard_map_func(o, features);
            return;
        case FunctionType::FT_Hadamard_Aggregate:
            call_hadamard_agg_func(o, features);
            return;
        default:
            return;
        }
    }
};

#endif // LUBAN_OPR_HPP