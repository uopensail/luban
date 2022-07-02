#ifndef LUBAN_LUBAN_H
#define LUBAN_LUBAN_H

#include "common.h"
#include <string>

class LuBan
{
private:
    void *toolkit;

public:
    LuBan() = delete;
    LuBan(const LuBan &) = delete;
    LuBan(const LuBan &&) = delete;
    LuBan(const char *config_file);
    ~LuBan();

    void process(const tensorflow::Features &features, std::vector<Entity *> &ret);
    void single_process(const tensorflow::Features &features, std::vector<Entity *> &ret);
    void bicross_process(const tensorflow::Features &featuresA,
                         const tensorflow::Features &featuresB, std::vector<Entity *> &ret);
};

#endif // LUBAN_LUBAN_H
