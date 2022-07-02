#include "basic_funcs.h"

void min_max(const tensorflow::Feature &feature, const ParamsHelper &params, Entity **entity)
{
    int size = value_size(feature);
    int gid = params.get<int>("gid");
    float max_value = params.get<double>("max");
    float min_value = params.get<double>("min");
    assert(max_value != min_value && size > 0);
    *entity = new_entity(EntityType::eNumerical, size, gid);
    float diff = max_value - min_value;
    for (int i = 0; i < size; i++)
    {
        (*entity)->data[i] = (get_float(feature, i) - min_value) / diff;
    }
}
void standardize(const tensorflow::Feature &feature, const ParamsHelper &params, Entity **entity)
{
    int size = value_size(feature);
    int gid = params.get<int>("gid");
    float mean_value = params.get<double>("mean");
    float std_value = params.get<double>("std");
    assert(std_value != 0 && size > 0);
    *entity = new_entity(EntityType::eNumerical, size, gid);
    for (int i = 0; i < size; i++)
    {
        (*entity)->data[i] = (get_float(feature, i) - mean_value) / std_value;
    }
}

void normalize(const tensorflow::Feature &feature, const ParamsHelper &params, Entity **entity)
{
    int size = value_size(feature);
    int gid = params.get<int>("gid");
    float norm_value = params.get<double>("norm");
    assert(norm_value > 0 && size > 0);
    *entity = new_entity(EntityType::eNumerical, size, gid);
    for (int i = 0; i < size; i++)
    {
        (*entity)->data[i] = get_float(feature, i) / norm_value;
    }
}

void binarize(const tensorflow::Feature &feature, const ParamsHelper &params, Entity **entity)
{
    int size = value_size(feature);
    assert(size > 0);
    int gid = params.get<int>("gid");
    float threshold_value = params.get<double>("threshold");
    *entity = new_entity(EntityType::eNumerical, size, gid);
    for (int i = 0; i < size; i++)
    {
        (*entity)->data[i] = get_float(feature, i) < threshold_value ? 0.0 : 1.0;
    }
}

void box_cox(const tensorflow::Feature &feature, const ParamsHelper &params, Entity **entity)
{
    int size = value_size(feature);
    int gid = params.get<int>("gid");
    assert(size > 0);
    float lambda_value = params.get<double>("lambda");
    *entity = new_entity(EntityType::eNumerical, size, gid);
    auto func = [&lambda_value](float v) -> float
    {
        assert(v > 0);
        return lambda_value == 0.0 ? logf(v) : (powf(v, lambda_value) - 1.0f) / lambda_value;
    };
    for (int i = 0; i < size; i++)
    {
        (*entity)->data[i] = func(get_float(feature, i));
    }
}

void power(const tensorflow::Feature &feature, const ParamsHelper &params, Entity **entity)
{
    int size = value_size(feature);
    int gid = params.get<int>("gid");
    float power_value = params.get<double>("power");
    assert(size > 0);
    *entity = new_entity(EntityType::eNumerical, size, gid);

    for (int i = 0; i < size; i++)
    {
        (*entity)->data[i] = powf(get_float(feature, i), power_value);
    }
}

void log(const tensorflow::Feature &feature, const ParamsHelper &params, Entity **entity)
{
    int size = value_size(feature);
    assert(size > 0);
    int gid = params.get<int>("gid");
    *entity = new_entity(EntityType::eNumerical, size, gid);
    for (int i = 0; i < size; i++)
    {
        (*entity)->data[i] = logf(get_float(feature, i));
    }
}