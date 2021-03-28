#ifndef __LUBAN_BOX_COMMON_H__
#define __LUBAN_BOX_COMMON_H__

#include <string>
#include <vector>
#include "cpptoml.h"
#include "feature.pb.h"
#include "helper.h"
#include "utils.h"


namespace luban {
    //这是单特征处理的函数
    using single_function = std::vector<u_int64_t> &&(*)(std::string &key, tensorflow::Feature &feature,
                                                         ParamsHelper &params);
    //这是交叉特征处理的函数
    using cross_function = std::vector<u_int64_t> &&(*)(std::string &keyA, tensorflow::Feature &featureA,
                                                        std::string &keyB, tensorflow::Feature &featureB,
                                                        ParamsHelper &params);
} // namespace luban

#endif //__LUBAN_BOX_COMMON_H__