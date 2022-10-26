#include "feature_operators.h"
#include <chrono>

float _add_1_0(float a, float b)
{
    return a + b;
}

float _add_0_1(float a, float b)
{
    return a + b;
}

float _add_1_1(float a, float b)
{
    return a + b;
}

float _sub_1_0(float a, float b)
{
    return a - b;
}

float _sub_0_1(float a, float b)
{
    return b - a;
}

float _sub_1_1(float a, float b)
{
    return a - b;
}

float _mul_1_0(float a, float b)
{
    return a * b;
}

float _mul_0_1(float a, float b)
{
    return a * b;
}

float _mul_1_1(float a, float b)
{
    return a * b;
}

float _div_1_0(float a, float b)
{
    return a / b;
}

float _div_0_1(float a, float b)
{
    return b / a;
}

float _div_1_1(float a, float b)
{
    return a / b;
}

int64_t _mod_1_0(int64_t a, int64_t b)
{
    return a % b;
}

int64_t _mod_0_1(int64_t a, int64_t b)
{
    return b % a;
}

int64_t _mod_1_1(int64_t a, int64_t b)
{
    return a % b;
}

float _pow_1_0(float a, float b)
{
    return powf(a, b);
}

float _pow_0_1(float a, float b)
{
    return powf(b, a);
}

float _pow_1_1(float a, float b)
{
    return powf(a, b);
}

float ln(float a)
{
    return logf(a);
}

int64_t timestamp()
{
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    return now.count();
}