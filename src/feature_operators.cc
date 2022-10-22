#include "feature_operators.h"

float s_map_add(float a, float b) { return a + b; }
float s_map_sub(float a, float b) { return a - b; }
float s_map_mul(float a, float b) { return a * b; }
float s_map_div(float a, float b) { return a / b; }
int64_t s_map_floor(float v) { return int64_t(floorf(v)); }
int64_t s_map_ceil(float v) { return int64_t(ceilf(v)); }
float s_map_log(float v) { return logf(v); }
float s_map_power(float a, float b) { return powerf(a, b); }
float s_map_exp(float v) { return expf(v); }