#ifndef LUBAN_TOOLKIT_H
#define LUBAN_TOOLKIT_H

#include "factory.h"

namespace luban {
    //单特征参数
    struct SingleParams {
        std::string key;
        std::string func;
        ParamsHelper params;
    };

    //交叉特征参数
    struct CrossParams {
        std::string keyA;
        std::string keyB;
        std::string func;
        ParamsHelper params;
    };

    class ToolKit {
    private:
        Factory factory;
        //支持一个特征多个不同的处理方式
        std::vector<struct SingleParams> single_configs;
        std::vector<struct CrossParams> cross_configs;

    public:
        ToolKit(std::string &config_file) {
            auto global_config = cpptoml::parse_file(config_file);
            auto single_tables = global_config->get_table_array("singe_features");
            auto cross_tables = global_config->get_table_array("cross_features");
            auto func_tables = global_config->get_table_array("third_functions");
            for (const auto &table : *func_tables) {
                ParamsHelper pfunc(table);
                factory.load(pfunc);
            }

            for (const auto &table : *single_tables) {
                std::string feature_key = table->get_as<std::string>("key").value_or("");
                std::string func_name = table->get_as<std::string>("func").value_or("");
                if (func_name == "" || feature_key == "") {
                    continue;
                }
                ParamsHelper p(table->get_table("params"));
                struct SingleParams single_params{feature_key, func_name, p};
                single_configs.push_back(single_params);
            }

            for (const auto &table : *cross_tables) {
                std::string feature_keya = table->get_as<std::string>("keyA").value_or("");
                std::string feature_keyb = table->get_as<std::string>("keyB").value_or("");

                std::string func_name = table->get_as<std::string>("func").value_or("");
                if (func_name == "" || feature_keya == "" || feature_keyb == "") {
                    continue;
                }
                ParamsHelper p(table->get_table("params"));
                struct CrossParams cross_params{feature_keya, feature_keyb, func_name, p};
                cross_configs.push_back(cross_params);
            }
        }

        ~ToolKit() {
            single_configs.clear();
            cross_configs.clear();
        }

        //单特征处理
        std::vector<u_int64_t> &&single_process(tensorflow::Features &features) {
            auto features_map = features.feature();
            std::vector<u_int64_t> ret;
            for (auto &v : single_configs) {
                auto iter = features_map.find(v.key);
                if (iter == features_map.end()) {
                    continue;
                }
                auto keys = factory.single_process(v.func, v.key, iter->second, v.params);
                append(ret, keys);
            }
            return std::move(ret);
        }

        //交叉特征处理
        std::vector<u_int64_t> &&cross_process(tensorflow::Features &features) {
            auto features_map = features.feature();
            std::vector<u_int64_t> ret;
            for (auto &v : cross_configs) {
                auto iterA = features_map.find(v.keyA);
                if (iterA == features_map.end()) {
                    continue;
                }

                auto iterB = features_map.find(v.keyB);
                if (iterB == features_map.end()) {
                    continue;
                }

                auto keys = factory.cross_process(v.func, v.keyA, iterA->second, v.keyB, iterB->second, v.params);
                append(ret, keys);
            }
            return std::move(ret);

        }

        //统一处理
        std::vector<u_int64_t> &&process(tensorflow::Features &features) {
            std::vector<u_int64_t> ret;
            auto single_keys = single_process(features);
            auto cross_keys = cross_process(features);
            append(ret, single_keys);
            append(ret, cross_keys);
            std::move(ret);
        }
    };
} // namespace luban
#endif //LUBAN_TOOLKIT_H
