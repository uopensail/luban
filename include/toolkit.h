#ifndef LUBAN_TOOLKIT_H
#define LUBAN_TOOLKIT_H

#pragma once

#include "basic_funcs.h"
#include "common.h"
#include <unordered_map>

class ToolKit
{
private:
    std::vector<SingleFuncParams> single_configs;
    std::vector<CrossFuncParams> cross_configs;
    std::unordered_map<std::string, single_func> single_funcs;
    std::unordered_map<std::string, cross_func> cross_funcs;

public:
    ToolKit() = delete;
    ToolKit(const ToolKit &) = delete;
    ToolKit(const ToolKit &&) = delete;
    ToolKit(const std::string &config_file);

    ~ToolKit();

    //单特征处理
    void single_process(const tensorflow::Features &features, std::vector<Entity *> &result);

    //交叉特征处理
    void cross_process(const tensorflow::Features &features, std::vector<Entity *> &result);

    //交叉特征处理
    void bicross_process(const tensorflow::Features &featuresA, const tensorflow::Features &featuresB,
                         std::vector<Entity *> &result);

    //统一处理
    void process(const tensorflow::Features &features, std::vector<Entity *> &result);
};

#endif // LUBAN_TOOLKIT_H
