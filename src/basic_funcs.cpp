#include "basic_funcs.h"

void hash(const tensorflow::Feature &feature, const ParamsHelper &params,
          Entity **entity)
{
    int size = value_size(feature);
    assert(size > 0);
    int gid = params.get<int>("gid");
    *entity = new_entity(EntityType::eCategorical, size, gid);
    for (int i = 0; i < size; i++)
    {
        (*entity)->index[i] = mask_gid(mmh3(get_string(feature, i)), gid);
    }
}

void bin(const tensorflow::Feature &feature, const ParamsHelper &params,
         Entity **entity)
{
    int size = value_size(feature);
    assert(size > 0);
    int gid = params.get<int>("gid");
    auto bin_values = params.get_array<double>("bin");
    *entity = new_entity(EntityType::eCategorical, size, gid);
    //将数据分箱
    auto get_bin = [&bin_values](float v) -> int64_t
    {
        auto start = bin_values.data();
        auto end = start + bin_values.size();
        return std::upper_bound(start, end, double(v)) - start;
    };

    for (int i = 0; i < size; i++)
    {
        (*entity)->index[i] = mask_gid(get_bin(get_float(feature, i)), gid);
    }
}

void logint(const tensorflow::Feature &feature,
            const ParamsHelper &params, Entity **entity)
{
    int size = value_size(feature);
    assert(size > 0);
    int gid = params.get<int>("gid");
    *entity = new_entity(EntityType::eCategorical, size, gid);
    for (int i = 0; i < size; i++)
    {
        (*entity)->index[i] = mask_gid(u_int64_t(logf(get_float(feature, i))), gid);
    }
}

void merge(const tensorflow::Feature &featureA,
           const tensorflow::Feature &featureB,
           const ParamsHelper &params, Entity **entity)
{
    int sizeA = value_size(featureA);
    int sizeB = value_size(featureB);
    assert(sizeA > 0 && sizeB > 0);
    int gid = params.get<int>("gid");
    *entity = new_entity(EntityType::eCategorical, sizeA * sizeB, gid);
    for (int i = 0; i < sizeA; i++)
    {
        auto tmpv = get_string(featureA, i);
        for (int j = 0; j < sizeB; j++)
        {
            (*entity)->index[i * sizeB + j] = mask_gid(mmh3(tmpv + "&" + get_string(featureB, j)), gid);
        }
    }
}