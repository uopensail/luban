#ifndef __LUBAN_BOX_COMMON_H__
#define __LUBAN_BOX_COMMON_H__

#include <string>
#include <vector>
#include "cpptoml.h"
#include "feature.pb.h"
#include "helper.h"
#include "utils.h"


namespace luban {
    namespace box {
        using single_function = std::vector<u_int64_t> &&(*)(std::string &key, tensorflow::Feature &feature,
                                                             ParamsHelper &params);
        using cross_function = std::vector<u_int64_t> &&(*)(std::string &keyA, tensorflow::Feature &featureA,
                                                            std::string &keyB, tensorflow::Feature &featureB,
                                                            ParamsHelper &params);
    } // namespace box
} // namespace luban

#endif //__LUBAN_BOX_COMMON_H__