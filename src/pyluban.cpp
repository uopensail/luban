#include "pyluban.h"
#include "feature.pb.h"
PyEntity::PyEntity() : data_(nullptr), size_(0) {}

void PyEntity::init(int size)
{
    assert(size > 0);
    data_ = (Entity **)calloc(size, sizeof(Entity *));
    size_ = size;
}

Entity *PyEntity::get(int index)
{
    if ((index >= 0) && (index < size_))
    {
        return data_[index];
    }
    return nullptr;
}

PyEntity ::~PyEntity()
{
    for (int i = 0; i < size_; i++)
    {
        safe_delete(data_[i]);
    }
    safe_delete(data_);
}

int PyEntity ::size()
{
    return size_;
}

PyToolKit::PyToolKit(std::string config_file)
{
    toolkit = new ToolKit(config_file);
}

PyToolKit::~PyToolKit()
{
    delete toolkit;
}

void PyToolKit::single_process(char *features, int len, PyEntity &entity)
{
    tensorflow::Features tf_features;
    tf_features.ParseFromArray(features, len);
    std::vector<Entity *> ret;
    toolkit->single_process(tf_features, ret);
    size_t size = ret.size();
    if (size == 0)
    {
        return;
    }
    entity.init(size);
    for (size_t i = 0; i < size; i++)
    {
        entity.data_[i] = ret[i];
    }
}

void PyToolKit::cross_process(char *features, int len, PyEntity &entity)
{
    tensorflow::Features tf_features;
    tf_features.ParseFromArray(features, len);
    std::vector<Entity *> ret;
    toolkit->cross_process(tf_features, ret);
    size_t size = ret.size();
    if (size == 0)
    {
        return;
    }
    entity.init(size);
    for (size_t i = 0; i < size; i++)
    {
        entity.data_[i] = ret[i];
    }
}

void PyToolKit::process(char *features, int len, PyEntity &entity)
{
    tensorflow::Features tf_features;
    tf_features.ParseFromArray(features, len);
    std::vector<Entity *> ret;
    toolkit->process(tf_features, ret);

    size_t size = ret.size();
    if (size == 0)
    {
        return;
    }
    entity.init(size);
    for (size_t i = 0; i < size; i++)
    {
        entity.data_[i] = ret[i];
    }
}

void PyToolKit::process_file(std::string input_file, std::string output_file)
{
}