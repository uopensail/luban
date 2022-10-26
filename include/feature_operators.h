#ifndef LUBAN_FEATURE_OPERATORS_H
#define LUBAN_FEATURE_OPERATORS_H

#include "feature_operator_handler.hpp"
#include <math.h>
#include <string>

float _add_1_0(float, float);
unary_map_function_wrapper(_add_1_0);

float _add_0_1(float, float);
unary_map_function_wrapper(_add_0_1);

float _add_1_1(float, float);
cartesian_cross_function_wrapper(_add_1_1);
hadamard_map_function_wrapper(_add_1_1);

float _sub_1_0(float, float);
unary_map_function_wrapper(_sub_1_0);

float _sub_0_1(float, float);
unary_map_function_wrapper(_sub_0_1);

float _sub_1_1(float, float);
cartesian_cross_function_wrapper(_sub_1_1);
hadamard_map_function_wrapper(_sub_1_1);

float _mul_1_0(float, float);
unary_map_function_wrapper(_mul_1_0);

float _mul_0_1(float, float);
unary_map_function_wrapper(_mul_0_1);

float _mul_1_1(float, float);
cartesian_cross_function_wrapper(_mul_1_1);
hadamard_map_function_wrapper(_mul_1_1);

float _div_1_0(float, float);
unary_map_function_wrapper(_div_1_0);

float _div_0_1(float, float);
unary_map_function_wrapper(_div_0_1);

float _div_1_1(float, float);
cartesian_cross_function_wrapper(_div_1_1);
hadamard_map_function_wrapper(_div_1_1);

int64_t _mod_1_0(int64_t a, int64_t b);
unary_map_function_wrapper(_mod_1_0);

int64_t _mod_0_1(int64_t a, int64_t b);
unary_map_function_wrapper(_mod_0_1);

int64_t _mod_1_1(int64_t a, int64_t b);
cartesian_cross_function_wrapper(_mod_1_1);
hadamard_map_function_wrapper(_mod_1_1);

float _pow_1_0(float, float);
unary_map_function_wrapper(_pow_1_0);

float _pow_0_1(float, float);
unary_map_function_wrapper(_pow_0_1);

float _pow_1_1(float, float);
cartesian_cross_function_wrapper(_pow_1_1);
hadamard_map_function_wrapper(_pow_1_1);

float min_max(float v, float min, float max);

unary_map_function_wrapper(floorf);
unary_map_function_wrapper(ceilf);
unary_map_function_wrapper(logf);
unary_map_function_wrapper(expf);

int64_t timestamp();
std::string substr(const std::string &str);

std::string concat(const std::string &a, const std::string &b);

#endif // LUBAN_FEATURE_OPERATORS_H