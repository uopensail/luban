//
// `LuBan` - 'c++ tool for transformating and hashing features'
// Copyright (C) 2019 - present timepi <timepi123@gmail.com>
//
// This file is part of `LuBan`.
// //
// `LuBan` is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// `LuBan` is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with `LuBan`.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef LUBAN_FEATURE_OPERATORS_H
#define LUBAN_FEATURE_OPERATORS_H

#pragma once

#include "feature_operator_handler.hpp"
#include <math.h>
#include <string>
#include <vector>

// add
float _add(float, float);
float _add_0_1(float, float);
unary_map_function_wrapper(_add);
unary_map_function_wrapper(_add_0_1);
cartesian_cross_function_wrapper(_add);
hadamard_map_function_wrapper(_add);

// sub
float _sub(float, float);
float _sub_0_1(float, float);
unary_map_function_wrapper(_sub);
unary_map_function_wrapper(_sub_0_1);
cartesian_cross_function_wrapper(_sub);
hadamard_map_function_wrapper(_sub);

// mul
float _mul(float, float);
float _mul_0_1(float, float);
unary_map_function_wrapper(_mul);
unary_map_function_wrapper(_mul_0_1);
cartesian_cross_function_wrapper(_mul);
hadamard_map_function_wrapper(_mul);

// div
float _div(float, float);
float _div_0_1(float, float);
unary_map_function_wrapper(_div);
unary_map_function_wrapper(_div_0_1);
cartesian_cross_function_wrapper(_div);
hadamard_map_function_wrapper(_div);

// mod
int64_t _mod(int64_t a, int64_t b);
int64_t _mod_0_1(int64_t a, int64_t b);
unary_map_function_wrapper(_mod);
unary_map_function_wrapper(_mod_0_1);
cartesian_cross_function_wrapper(_mod);
hadamard_map_function_wrapper(_mod);

// pow
float _pow(float, float);
float _pow_0_1(float, float);
unary_map_function_wrapper(_pow);
unary_map_function_wrapper(_pow_0_1);
cartesian_cross_function_wrapper(_pow);
hadamard_map_function_wrapper(_pow);

unary_map_function_wrapper(floorf);
unary_map_function_wrapper(ceilf);
unary_map_function_wrapper(logf);
unary_map_function_wrapper(expf);

int64_t timestamp();
std::string date();

float min_max(float v, float min, float max);
unary_map_function_wrapper(min_max);

float z_score(float v, float mean, float stdv);
unary_map_function_wrapper(z_score);

float binarize(float v, float threshold);
unary_map_function_wrapper(binarize);

void box_cox(float v, float lambda);
unary_map_function_wrapper(box_cox);

std::string substr(const std::string &str, size_t pos, size_t len);
unary_map_function_wrapper(substr);

std::string concat(const std::string &a, const std::string &b);
std::string concat_0_1(const std::string &a, const std::string &b);
unary_map_function_wrapper(concat);
unary_map_function_wrapper(concat_0_1);
cartesian_cross_function_wrapper(concat);
hadamard_map_function_wrapper(concat);

void normalize(std::vector<float> &dst, const std::vector<float> &src, float norm);
unary_agg_function_wrapper(normalize);

void topk(std::vector<std::string> &dst, const std::vector<std::string> &src, float k);
unary_agg_function_wrapper(topk);

#endif // LUBAN_FEATURE_OPERATORS_H