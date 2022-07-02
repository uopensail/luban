#include "toolkit.h"
#include <iostream>
ToolKit::ToolKit(const std::string &config_file)
{

    single_funcs["hash"] = hash;
    single_funcs["bin"] = bin;
    single_funcs["logint"] = logint;
    single_funcs["standardize"] = standardize;
    single_funcs["normalize"] = normalize;
    single_funcs["binarize"] = binarize;
    single_funcs["min_max"] = min_max;
    single_funcs["box_cox"] = box_cox;
    single_funcs["power"] = power;
    single_funcs["log"] = log;

    cross_funcs["merge"] = merge;

    auto global_config = cpptoml::parse_file(config_file);
    auto single_tables = global_config->get_table_array("single_features");
    auto cross_tables = global_config->get_table_array("cross_features");

    if (single_tables != nullptr)
    {
        for (const auto &table : *single_tables)
        {
            std::string feature_key = table->get_as<std::string>("key").value_or("");
            std::string func_name = table->get_as<std::string>("func").value_or("");
            if (func_name == "" || feature_key == "")
            {
                continue;
            }
            ParamsHelper p(table->get_table("params"));
            single_configs.emplace_back(SingleFuncParams{feature_key, func_name, p});
        }
    }

    if (cross_tables != nullptr)
    {
        for (const auto &table : *cross_tables)
        {
            std::string feature_keya = table->get_as<std::string>("keyA").value_or("");
            std::string feature_keyb = table->get_as<std::string>("keyB").value_or("");

            std::string func_name = table->get_as<std::string>("func").value_or("");
            if (func_name == "" || feature_keya == "" || feature_keyb == "")
            {
                continue;
            }
            ParamsHelper p(table->get_table("params"));
            cross_configs.emplace_back(CrossFuncParams{feature_keya, feature_keyb, func_name, p});
        }
    }

}

ToolKit::~ToolKit()
{
    single_configs.clear();
    cross_configs.clear();
    cross_funcs.clear();
    single_funcs.clear();
}

//单特征处理
void ToolKit::single_process(const tensorflow::Features &features, std::vector<Entity *> &result)
{
    auto features_map = features.feature();
    for (auto &v : single_configs)
    {
        auto iter = features_map.find(v.key);
        if (iter == features_map.end())
        {
            continue;
        }

        auto func_iter = single_funcs.find(v.func);
        if (func_iter == single_funcs.end())
        {
            continue;
        }

        single_func foo = func_iter->second;
        Entity *entity = nullptr;
        foo(iter->second, v.params, &entity);
        std::cout << entity->index[0]<<std::endl;
         if (entity != nullptr)
        {
            result.push_back(entity);
        }
    }
    return;
}

//交叉特征处理
void ToolKit::cross_process(const tensorflow::Features &features, std::vector<Entity *> &result)
{
    auto features_map = features.feature();
    for (auto &v : cross_configs)
    {
        auto iterA = features_map.find(v.keyA);
        if (iterA == features_map.end())
        {
            continue;
        }

        auto iterB = features_map.find(v.keyB);
        if (iterB == features_map.end())
        {
            continue;
        }
        auto iter = cross_funcs.find(v.func);
        if (iter == cross_funcs.end())
        {
            continue;
        }
        cross_func foo = iter->second;
        Entity *entity = nullptr;
        foo(iterA->second, iterB->second, v.params, &entity);
        if (entity != nullptr)
        {
            result.push_back(entity);
        }
    }
    return;
}

//交叉特征处理
void ToolKit::bicross_process(const tensorflow::Features &featuresA, const tensorflow::Features &featuresB,
                              std::vector<Entity *> &result)
{
    auto features_map_A = featuresA.feature();
    auto features_map_B = featuresB.feature();
    for (auto &v : cross_configs)
    {
        auto iterA = features_map_A.find(v.keyA);
        auto iterB = features_map_B.find(v.keyB);
        if (iterA == features_map_A.end())
        {
            iterA = features_map_B.find(v.keyA);
            if (iterA == features_map_B.end())
            {
                //都没有找到
                continue;
            }
            iterB = features_map_A.find(v.keyB);
            if (iterB == features_map_A.end())
            {
                //都没有找到
                continue;
            }
        }
        else if (iterB == features_map_B.end())
        {
            //都没有找到
            continue;
        }

        auto iter = cross_funcs.find(v.func);
        if (iter == cross_funcs.end())
        {
            continue;
        }
        cross_func foo = iter->second;
        Entity *entity = nullptr;
        foo(iterA->second, iterB->second, v.params, &entity);

        if (entity != nullptr)
        {
            result.push_back(entity);
        }
    }
    return;
}

//统一处理
void ToolKit::process(const tensorflow::Features &features, std::vector<Entity *> &result)
{
    single_process(features, result);
    cross_process(features, result);
}
