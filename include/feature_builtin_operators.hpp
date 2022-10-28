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

#ifndef LUBAN_FEATURE_BUILTIN_OPERATORS_HPP
#define LUBAN_FEATURE_BUILTIN_OPERATORS_HPP

#pragma once

#include "feature_operator_handler.hpp"
#include <math.h>
#include <string>
#include <vector>

// add
static float _add(float a, float b)
{
    return a + b;
}
static float _add_0_1(float a, float b)
{
    return a + b;
}
unary_map_function_wrapper(_add);
unary_map_function_wrapper(_add_0_1);
cartesian_cross_function_wrapper(_add);
hadamard_map_function_wrapper(_add);

// sub
static float _sub(float a, float b)
{
    return a - b;
}
static float _sub_0_1(float a, float b)
{
    return b - a;
}
unary_map_function_wrapper(_sub);
unary_map_function_wrapper(_sub_0_1);
cartesian_cross_function_wrapper(_sub);
hadamard_map_function_wrapper(_sub);

// mul
static float _mul(float a, float b)
{
    return a * b;
}
static float _mul_0_1(float a, float b)
{
    return a * b;
}
unary_map_function_wrapper(_mul);
unary_map_function_wrapper(_mul_0_1);
cartesian_cross_function_wrapper(_mul);
hadamard_map_function_wrapper(_mul);

// div
static float _div(float a, float b)
{
    return a / b;
}
static float _div_0_1(float a, float b)
{
    return b / a;
}
unary_map_function_wrapper(_div);
unary_map_function_wrapper(_div_0_1);
cartesian_cross_function_wrapper(_div);
hadamard_map_function_wrapper(_div);

// mod
static int64_t _mod(int64_t a, int64_t b)
{
    return a % b;
}
static int64_t _mod_0_1(int64_t a, int64_t b)
{
    return b % a;
}
unary_map_function_wrapper(_mod);
unary_map_function_wrapper(_mod_0_1);
cartesian_cross_function_wrapper(_mod);
hadamard_map_function_wrapper(_mod);

// pow
static float _pow(float a, float b)
{
    return powf(a, b);
}
static float _pow_0_1(float a, float b)
{
    return powf(a, b);
}
unary_map_function_wrapper(_pow);
unary_map_function_wrapper(_pow_0_1);
cartesian_cross_function_wrapper(_pow);
hadamard_map_function_wrapper(_pow);

unary_map_function_wrapper(floorf);
unary_map_function_wrapper(ceilf);
unary_map_function_wrapper(logf);
unary_map_function_wrapper(expf);

static int64_t timestamp()
{
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    return now.count();
}
static std::string date()
{
    time_t t = time(0);
    char tmp[9] = {0};
    strftime(tmp, sizeof(tmp), "%Y%m%d", localtime(&t));
    return std::string((char *)tmp);
}

static float min_max(float v, float min, float max)
{
    return (v - min) / (max - min);
}
unary_map_function_wrapper(min_max);

static float z_score(float v, float mean, float stdv)
{
    return (v - mean) / stdv;
}
unary_map_function_wrapper(z_score);

static int64_t binarize(float v, float threshold)
{

    return v < threshold ? 0.0 : 1.0;
}

unary_map_function_wrapper(binarize);

static float box_cox(float v, float lambda)
{
    return lambda == 0.0 ? logf(v) : (powf(v, lambda) - 1.0f) / lambda;
}
unary_map_function_wrapper(box_cox);

static std::string substr( std::string str, size_t pos, size_t len)
{
    return str.substr(pos, len);
}
unary_map_function_wrapper(substr);

static std::string concat( std::string a,  std::string b)
{
    std::string v;
    v.reserve(a.size() + b.size());
    v += a;
    v += b;
    return v;
}
static std::string concat_0_1( std::string a,  std::string b)
{
    std::string v;
    v.reserve(a.size() + b.size());
    v += b;
    v += a;
    return v;
}
unary_map_function_wrapper(concat);
unary_map_function_wrapper(concat_0_1);
cartesian_cross_function_wrapper(concat);
hadamard_map_function_wrapper(concat);

static void normalize(std::vector<float> &dst,   std::vector<float> &src, float norm)
{
    assert(norm >= 1);
    dst.reserve(src.size());
    float norm_value = 0.0;
    for (auto &v : src)
    {
        norm_value += powf(abs(v), norm);
    }
    norm_value = powf(norm_value, 1.0 / norm);

    for (auto &v : src)
    {
        dst.push_back(v / norm_value);
    }
}
unary_agg_function_wrapper(normalize);

static void topk(std::vector<std::string> &dst,   std::vector<std::string> &src, size_t k)
{
    dst.clear();
    dst.reserve(k);
    size_t count = 0;
    for (auto &v : src)
    {
        if (count < k)
        {
            dst.push_back(v);
            count++;
        }
    }
}
unary_agg_function_wrapper(topk);

#endif // LUBAN_FEATURE_BUILTIN_OPERATORS_HPP