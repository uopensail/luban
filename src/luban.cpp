#include "luban.h"
#include "toolkit.h"

LuBan::LuBan(const char *config_file)
{
    std::string config(config_file);
    toolkit = new ToolKit(config);
}

LuBan::~LuBan()
{
    delete (ToolKit *)toolkit;
}

void LuBan::process(const tensorflow::Features &features, std::vector<Entity *> &ret)
{
    auto toolkit_ptr = (ToolKit *)toolkit;
    toolkit_ptr->process(features, ret);
}

void LuBan::single_process(const tensorflow::Features &features, std::vector<Entity *> &ret)
{
    auto toolkit_ptr = (ToolKit *)toolkit;
    toolkit_ptr->single_process(features, ret);
}

void LuBan::bicross_process(const tensorflow::Features &featuresA,
                            const tensorflow::Features &featuresB, std::vector<Entity *> &ret)
{
    auto toolkit_ptr = (ToolKit *)toolkit;
    toolkit_ptr->bicross_process(featuresA, featuresB, ret);
}