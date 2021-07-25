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
            auto single_tables = global_config->get_table_array("single_features");
            auto cross_tables = global_config->get_table_array("cross_features");

            if (single_tables != nullptr) {
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
            }

            if (cross_tables != nullptr) {
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
        }

        ~ToolKit() {
            single_configs.clear();
            cross_configs.clear();
        }

        //单特征处理
        void single_process(const tensorflow::Features &features, std::vector<u_int64_t> &ret) {
            auto features_map = features.feature();
            for (auto &v : single_configs) {
                auto iter = features_map.find(v.key);
                if (iter == features_map.end()) {
                    continue;
                }
                factory.single_process(v.func, v.key, iter->second, v.params, ret);
            }
            return;
        }

        //交叉特征处理
        void cross_process(const tensorflow::Features &features, std::vector<u_int64_t> &ret) {
            auto features_map = features.feature();
            for (auto &v : cross_configs) {
                auto iterA = features_map.find(v.keyA);
                if (iterA == features_map.end()) {
                    continue;
                }

                auto iterB = features_map.find(v.keyB);
                if (iterB == features_map.end()) {
                    continue;
                }

                factory.cross_process(v.func, v.keyA, iterA->second, v.keyB, iterB->second, v.params, ret);

            }
            return;

        }

        //交叉特征处理
        void bicross_process(tensorflow::Features &featuresA, tensorflow::Features &featuresB,
                             std::vector<u_int64_t> &ret) {
            auto features_map_A = featuresA.feature();
            auto features_map_B = featuresB.feature();
            for (auto &v : cross_configs) {
                auto iterA = features_map_A.find(v.keyA);
                auto iterB = features_map_B.find(v.keyB);
                if (iterA == features_map_A.end()) {
                    iterA = features_map_B.find(v.keyA);
                    if (iterA == features_map_B.end()) {
                        //都没有找到
                        continue;
                    }
                    iterB = features_map_A.find(v.keyB);
                    if (iterB == features_map_A.end()) {
                        //都没有找到
                        continue;
                    }
                } else if (iterB == features_map_B.end()) {
                    //都没有找到
                    continue;
                }

                factory.cross_process(v.func, v.keyA, iterA->second, v.keyB, iterB->second, v.params, ret);

            }
            return;

        }

        //统一处理
        void process(const tensorflow::Features &features, std::vector<u_int64_t> &ret) {
            single_process(features, ret);

            cross_process(features, ret);
        }
    };
} // namespace luban
#endif //LUBAN_TOOLKIT_H
