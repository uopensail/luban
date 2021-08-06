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
        ToolKit(std::string &config_file);

        ~ToolKit();

        //单特征处理
        void single_process(const tensorflow::Features &features, std::vector<u_int64_t> &ret);

        //交叉特征处理
        void cross_process(const tensorflow::Features &features, std::vector<u_int64_t> &ret);

        //交叉特征处理
        void bicross_process(tensorflow::Features &featuresA, tensorflow::Features &featuresB,
                             std::vector<u_int64_t> &ret);

        //统一处理
        void process(const tensorflow::Features &features, std::vector<u_int64_t> &ret);
    };
} // namespace luban
#endif //LUBAN_TOOLKIT_H
