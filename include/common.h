#ifndef __LUBAN_BOX_COMMON_H__
#define __LUBAN_BOX_COMMON_H__

#include "cpptoml.h"
#include "feature.pb.h"
#include "helper.h"
#include "utils.h"
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>

//单特征处理的函数
using single_func = void (*)(const tensorflow::Feature &feature,
                             const ParamsHelper &params, Entity **entity);
//交叉特征处理的函数
using cross_func = void (*)(const tensorflow::Feature &featureA,
                            const tensorflow::Feature &featureB,
                            const ParamsHelper &params, Entity **entity);

#endif //__LUBAN_BOX_COMMON_H__