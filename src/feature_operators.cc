#include "feature_operators.h"
#include <chrono>
#include <time.h>
float _add(float a, float b)
{
    return a + b;
}

float _add_0_1(float a, float b)
{
    return a + b;
}

float _sub(float a, float b)
{
    return a - b;
}

float _sub_0_1(float a, float b)
{
    return b - a;
}

float _mul(float a, float b)
{
    return a * b;
}

float _mul_0_1(float a, float b)
{
    return a * b;
}

float _div(float a, float b)
{
    return a / b;
}

float _div_0_1(float a, float b)
{
    return b / a;
}

int64_t _mod(int64_t a, int64_t b)
{
    return a % b;
}

int64_t _mod_0_1(int64_t a, int64_t b)
{
    return b % a;
}

float _pow(float a, float b)
{
    return powf(a, b);
}

float _pow_0_1(float a, float b)
{
    return powf(b, a);
}

int64_t timestamp()
{
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    return now.count();
}

std::string date()
{
    time_t t = time(0);
    char tmp[9] = {0};
    strftime(tmp, sizeof(tmp), "%Y%m%d", localtime(&t));
    return std::string((char *)tmp);
}

float min_max(float v, float min, float max)
{
    return (v - min) / (max - min);
}

float z_score(float v, float mean, float stdv)
{
    return (v - mean) / stdv;
}

float binarize(float v, float threshold)
{

    return v < threshold ? 0.0 : 1.0;
}

void box_cox(float v, float lambda)
{
    return lambda == 0.0 ? logf(v) : (powf(v, lambda) - 1.0f) / lambda;
}

std::string substr(const std::string &str, size_t pos, size_t len)
{
    return str.substr(pos, len);
}

std::string concat(const std::string &a, const std::string &b)
{
    std::string v;
    v.reserve(a.size() + b.size());
    v += a;
    v += b;
    return v;
}

std::string concat_0_1(const std::string &a, const std::string &b)
{
    std::string v;
    v.reserve(a.size() + b.size());
    v += b;
    v += a;
    return v;
}

void normalize(std::vector<float> &dst, const std::vector<float> &src, float norm)
{
    assert(norm >= 1);
    ret.reserve(values.size());
    float norm_value = 0.0;
    for (auto &v : values)
    {
        norm_value += powf(abs(v), norm);
    }
    norm_value = powf(norm_value, 1.0 / norm);

    for (auto &v : values)
    {
        ret.push_back(v / norm_value);
    }
}

void topk(std::vector<std::string> &dst, const std::vector<std::string> &src, size_t k)
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