#include "luban_c.h"
#include "toolkit.h"

void *luban_new(const char *config_file)
{
    return new ToolKit(config_file);
}

void luban_release(void *ptr)
{
    if (ptr != nullptr)
    {
        delete (ToolKit *)ptr;
        ptr = nullptr;
    }
}

void *luban_process(void *ptr, char *feature, int feature_len, void *return_len)
{
    assert(ptr != nullptr && feature != nullptr && feature_len > 0);
    auto toolkit = (ToolKit *)ptr;
    tensorflow::Features features;
    features.ParseFromArray(feature, feature_len);
    std::vector<Entity *> ret;
    toolkit->process(features, ret);
    size_t total_size = 0, offset = 0, size = 0;
    for (size_t i = 0; i < ret.size(); i++)
    {
        total_size += get_entity_size(ret[i]);
    }
    if (total_size == 0)
    {
        *((int *)return_len) = 0;
        return nullptr;
    }
    char *data = (char *)malloc(total_size);
    for (size_t i = 0; i < ret.size(); i++)
    {
        size = get_entity_size(ret[i]);
        memcpy(data + offset, ret[i], size);
        offset += size;
        safe_delete(ret[i]);
    }
    *((int *)return_len) = total_size;
    return data;
}

void *luban_single_process(void *ptr, char *feature, int feature_len, void *return_len)
{
    assert(ptr != nullptr && feature != nullptr && feature_len > 0);
    auto toolkit = (ToolKit *)ptr;
    tensorflow::Features features;
    features.ParseFromArray(feature, feature_len);
    std::vector<Entity *> ret;
    toolkit->single_process(features, ret);
    size_t total_size = 0, offset = 0, size = 0;
    for (size_t i = 0; i < ret.size(); i++)
    {
        total_size += get_entity_size(ret[i]);
    }
    if (total_size == 0)
    {
        *((int *)return_len) = 0;
        return nullptr;
    }
    char *data = (char *)malloc(total_size);
    for (size_t i = 0; i < ret.size(); i++)
    {
        size = get_entity_size(ret[i]);
        memcpy(data + offset, ret[i], size);
        offset += size;
        safe_delete(ret[i]);
    }
    *((int *)return_len) = total_size;
    return data;
}

void *luban_bicross_process(void *ptr, char *featureA, int feature_lenA,
                            char *featureB, int feature_lenB, void *return_len)
{
    assert(ptr != nullptr && feature != nullptr && feature_len > 0);
    auto toolkit = (ToolKit *)ptr;
    tensorflow::Features featuresA, featuresB;
    featuresA.ParseFromArray(featureA, feature_lenA);
    featuresB.ParseFromArray(featureB, feature_lenB);
    std::vector<Entity *> ret;
    toolkit->bicross_process(featuresA, featuresB, ret);
    size_t total_size = 0, offset = 0, size = 0;
    for (size_t i = 0; i < ret.size(); i++)
    {
        total_size += get_entity_size(ret[i]);
    }
    if (total_size == 0)
    {
        *((int *)return_len) = 0;
        return nullptr;
    }
    char *data = (char *)malloc(total_size);
    for (size_t i = 0; i < ret.size(); i++)
    {
        size = get_entity_size(ret[i]);
        memcpy(data + offset, ret[i], size);
        offset += size;
        safe_delete(ret[i]);
    }
    *((int *)return_len) = total_size;
    return data;
}