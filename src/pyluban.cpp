#include "pyluban.h"
#include "feature.pb.h"
#include <iostream>
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
std::vector<float> PyEntity::get_data(int index)
{
    std::vector<float> ret;
    if ((index >= 0) && (index < size_) && (data_[index]->type == EntityType::eNumerical))
    {
        for (int i = 0; i < data_[index]->size; i++)
        {
            ret.push_back(data_[index]->data[i]);
        }
    }
    return ret;
}

std::vector<unsigned long long> PyEntity::get_index(int index)
{
    std::vector<unsigned long long> ret;
    std::cout<<"get_index: "<<index<<" size_ "<<size_<<" size: "<<data_[index]->size<<std::endl;
    if ((index >= 0) && (index < size_)&& (data_[index]->type == EntityType::eCategorical))
    {
        for (int i = 0; i < data_[index]->size; i++)
        {
            std::cout<<"get_index: "<<data_[index]->index[i]<<std::endl;
            ret.push_back(data_[index]->index[i]);
        }
    }
    return ret;
}

PyEntity ::~PyEntity()
{
    for (int i = 0; i < size_; i++)
    {
        std::cout << "delete: " << i << std::endl;
        //safe_delete(data_[i]);
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
    std::cout<<"PyToolKit::single_process: "<<ret.size()<<std::endl;
    size_t size = ret.size();
    if (size == 0)
    {
        return;
    }
    entity.init(size);
    for (size_t i = 0; i < size; i++)
    {
        std::cout<<"type: "<<ret[i]->type<<" gid: "<<ret[i]->gid<<std::endl;
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
     std::cout<<"PyToolKit::process: "<<ret.size()<<std::endl;
    if (size == 0)
    {
        return;
    }
    entity.init(size);
    for (size_t i = 0; i < size; i++)
    {
        entity.data_[i] = ret[i];
        std::cout<<"type: "<<entity.data_[i]->type<<" ret: "<<entity.data_[i]->gid<<std::endl;
        print_entity(ret[i]);
    }
}
