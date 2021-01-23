#ifndef __LUBAN_BOX_COMMON_H__
#define __LUBAN_BOX_COMMON_H__
#include "cpptoml.h"
#include "feature.pb.h"
#include "helper.h"
#include "utils.h"
#include <string>

namespace luban
{
    namespace box
    {
        using single_function = uint64_t && (std::string & key, tensorflow::Feature &feature, ParamsHelper &params);
        using cross_function = uint64_t && (std::string & keyA, tensorflow::Feature &featureA, std::string &keyB, tensorflow::Feature &featureB, ParamsHelper &params);
    } // namespace box
} // namespace luban

#endif //__LUBAN_BOX_COMMON_H__