#include "utils.h"
#include <chrono>

//把gid添加到hashid中的前8位
u_int64_t mask_gid(u_int64_t hash_id, int gid)
{
    assert(gid >= 0 && gid < MAX_GID);
    return (u_int64_t(gid) << (64 - FEATURE_GID_BITS)) + (hash_id & FEAUTRE_HASH_MASK);
}

void safe_delete(void *ptr)
{
    if (ptr != nullptr)
    {
        free(ptr);
        ptr = nullptr;
    }
}

//字符串split
void split(const std::string &s, std::vector<std::string> &tockens, const std::string &delimiters)
{
    std::string::size_type lastPos = s.find_first_not_of(delimiters, 0);
    std::string::size_type pos = s.find_first_of(delimiters, lastPos);
    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        tockens.emplace_back(s.substr(lastPos, pos - lastPos));
        lastPos = s.find_first_not_of(delimiters, pos);
        pos = s.find_first_of(delimiters, lastPos);
    }
}

// hash
u_int64_t mmh3(const std::string &key)
{
    u_int64_t ret[2];
    MurmurHash3_x64_128(key.c_str(), key.size(), 0, &ret);
    return ret[0];
}

u_int64_t mmh3(const std::string &&key)
{
    u_int64_t ret[2];
    MurmurHash3_x64_128(key.c_str(), key.size(), 0, &ret);
    return ret[0];
}

//获得当前的时间戳
u_int64_t get_current_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (u_int64_t)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
//新建entity
Entity *new_entity(EntityType type, size_t size, size_t gid)
{
    Entity *entity = nullptr;
    switch (type)
    {
    case EntityType::eCategorical:
        entity = (Entity *)malloc(sizeof(Entity) + sizeof(u_int64_t) * size);
        break;
    case EntityType::eNumerical:
        entity = (Entity *)malloc(sizeof(Entity) + sizeof(float) * size);
        break;
    default:
        return entity;
    }
    entity->type = type;
    entity->size = size;
    entity->gid = gid;
    return entity;
}

size_t get_entity_size(const Entity *entity)
{
    if (entity == nullptr)
    {
        return 0;
    }
    switch (entity->type)
    {
    case EntityType::eCategorical:
        return sizeof(Entity) + sizeof(u_int64_t) * entity->size;
    case EntityType::eNumerical:
        return sizeof(Entity) + sizeof(float) * entity->size;
    default:
        return 0;
    }
}

//打印数据
void print_entity(Entity *entity)
{
    std::cout << "[";
    if (entity == nullptr)
    {
        std::cout << "]" << std::endl;
        return;
    }
    if (entity->type == EntityType::eNumerical)
    {
        for (size_t i = 0; i < entity->size; i++)
        {
            std::cout << entity->data[i];
            if (i != entity->size - 1)
            {
                std::cout << ",";
            }
        }
    }
    else if (entity->type == EntityType::eNumerical)
    {
        for (size_t i = 0; i < entity->size; i++)
        {
            std::cout << entity->index[i];
            if (i != entity->size - 1)
            {
                std::cout << ",";
            }
        }
    }
    std::cout << "]" << std::endl;
}

void split_file(std::string filename, int num)
{
    std::cout << "Reading File: " << filename << std::endl;
    std::ofstream *fds[num];
    for (int i = 0; i < num; i++)
    {
        fds[i] = new std::ofstream(filename + "_" + std::to_string(i), std::ios::out);
    }
    std::ifstream reader(filename, std::ios::in);
    if (!reader)
    {
        exit(-1);
    }
    int count = 0;
    std::string line;
    while (getline(reader, line))
    {
        count++;
        if (count % 1000 == 0)
        {
            std::cout << "Reading File: " << filename << " Line No." << count << std::endl;
        }
        line += "\n";
        fds[rand() % num]->write(line.c_str(), line.size());
    }
    std::cout << "Finish Reading File: " << filename << " Total Line Count: " << count << std::endl;
    reader.close();
    for (int i = 0; i < num; i++)
    {
        fds[i]->close();
    }
}