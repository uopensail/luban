#ifndef __LUBAN_BOX_H__
#define __LUBAN_BOX_H__
#include "factory.h"
namespace luban
{
    namespace box
    {
        class ToolKit
        {
        private:
            Factory factory;
            //支持一个特征多个不同的处理方式
            std::unordered_map<std::string, std::vector<ParamsHelper>> single_configs;
            std::unordered_map<std::string, std::unordered_map<std::string, std::vector<ParamsHelper>>> cross_configs;

        public:
            ToolKit(std::string &config_file)
            {
                auto global_config = cpptoml::parse_file(config_file);
                auto single_tables = global_config->get_table_array("singe_features");
                auto cross_tables = global_config->get_table_array("cross_features");
                auto func_tables = global_config->get_table_array("third_functions");
                for (const auto &table : *func_tables)
                {
                    ParamsHelper pfunc(table);
                    factory.load(pfunc);
                }

                for (const auto &table : *single_tables)
                {
                    std::string feature_key = table->get_as<std::string>("key").value_or("");
                    std::string func_name = table->get_as<std::string>("func").value_or("");
                    if (func_name == "" || feature_key == "")
                    {
                        continue;
                    }
                    auto iter = single_configs.find(feature_key);
                    ParamsHelper p(table->get_table("params"));
                    if (iter == single_configs.end())
                    {
                        std::vector<ParamsHelper> params;
                        params.push_back(p);
                        single_configs[feature_key] = params;
                    }
                    else
                    {
                        iter->second.push_back(p);
                    }
                }

                for (const auto &table : *cross_tables)
                {
                    std::string feature_keya = table->get_as<std::string>("keyA").value_or("");
                    std::string feature_keyb = table->get_as<std::string>("keyB").value_or("");
                    
                    std::string func_name = table->get_as<std::string>("func").value_or("");
                    if (func_name == ""||feature_keya == "" || feature_keyb == "")
                    {
                        continue;
                    }
                    auto iter = cross_configs.find(feature_keya);
                    ParamsHelper p(table->get_table("params"));
                    if (iter == cross_configs.end())
                    {
                        std::vector<ParamsHelper> params;
                        params.push_back(p);
                        std::unordered_map<std::string, std::vector<ParamsHelper>> key_params;
                        key_params[feature_keyb] = params;
                        cross_configs[feature_keya] = key_params;
                    }
                    else
                    {
                        auto second_iter = iter->second.find(feature_keyb);
                        if(second_iter == iter->second.end()){
                            std::vector<ParamsHelper> params;
                            params.push_back(p);
                            iter->second[feature_keyb] = params;
                        }else{
                            second_iter->second.push_back(p);
                        }
                    }
                }
            }
        };
    } // namespace box
} // namespace luban
#endif //__LUBAN_BOX_H__
