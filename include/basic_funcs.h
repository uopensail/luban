#ifndef LUBAN_BASIC_FUNCS_H
#define LUBAN_BASIC_FUNCS_H

#pragma once

#include "common.h"
#include "helper.h"
#include "utils.h"
#include <math.h>

//处理成数值类型
// z-score
void z_score(const tensorflow::Feature &feature, const ParamsHelper &params, Entity **entity);
// l1,l2,lp
void normalize(const tensorflow::Feature &feature, const ParamsHelper &params, Entity **entity);
// 1 or 0
void binarize(const tensorflow::Feature &feature, const ParamsHelper &params, Entity **entity);
//[0~1]
void min_max(const tensorflow::Feature &feature, const ParamsHelper &params, Entity **entity);
// box-cox
void box_cox(const tensorflow::Feature &feature, const ParamsHelper &params, Entity **entity);
// x^y
void power(const tensorflow::Feature &feature, const ParamsHelper &params, Entity **entity);
// ln(x)
void log(const tensorflow::Feature &feature, const ParamsHelper &params, Entity **entity);
// exp(x)
void exp(const tensorflow::Feature &feature, const ParamsHelper &params, Entity **entity);
//将值转化为字符串后hash
void hash(const tensorflow::Feature &feature, const ParamsHelper &params, Entity **entity);
//分箱
void bin(const tensorflow::Feature &feature, const ParamsHelper &params, Entity **entity);
//对数取整
void logint(const tensorflow::Feature &feature, const ParamsHelper &params, Entity **entity);
//交叉特征处理，值合并后hash
void merge(const tensorflow::Feature &featureA,
           const tensorflow::Feature &featureB,
           const ParamsHelper &params, Entity **entity);

#endif // LUBAN_BASIC_BIN_H
