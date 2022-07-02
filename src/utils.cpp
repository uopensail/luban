#include "utils.h"

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

u_int64_t get_current_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (u_int64_t)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

Entity *new_entity(EntityType type, int size, int gid)
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