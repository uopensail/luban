#ifndef LUBAN_BOX_COMMON_H
#define LUBAN_BOX_COMMON_H

#pragma once

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

#endif //LUBAN_BOX_COMMON_H