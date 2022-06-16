#include "luban.h"
#include "toolkit.h"

LuBan::LuBan(std::string config_file)
{
    toolkit = new luban::ToolKit(config_file);
}

LuBan::~LuBan()
{
    delete (luban::ToolKit *)toolkit;
}

void LuBan::process(tensorflow::Features &features, std::vector<u_int64_t> &ret)
{
    auto toolkit_ptr = (luban::ToolKit *)toolkit;
    toolkit_ptr->process(features, ret);
}

void LuBan::single_process(tensorflow::Features &features, std::vector<u_int64_t> &ret)
{
    auto toolkit_ptr = (luban::ToolKit *)toolkit;
    toolkit_ptr->single_process(features, ret);
}

void LuBan::bicross_process(tensorflow::Features &featuresA,
                            tensorflow::Features &featuresB, std::vector<u_int64_t> &ret)
{
    auto toolkit_ptr = (luban::ToolKit *)toolkit;
    toolkit_ptr->bicross_process(featuresA, featuresB, ret);
}