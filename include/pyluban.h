#ifndef LUBAN_PYLUBAN_H
#define LUBAN_PYLUBAN_H

#include "toolkit.h"
#include <string>
#include <vector>

class PyToolKit;

class PyEntity
{
private:
    Entity **data_;
    int size_;
    friend PyToolKit;

private:
    void init(int size);

public:
    PyEntity();
    ~PyEntity();
    int size();
    Entity *get(int index);
};

class PyToolKit
{
private:
    ToolKit *toolkit;

public:
    PyToolKit() = delete;
    PyToolKit(const PyToolKit &) = delete;
    PyToolKit(const PyToolKit &&) = delete;
    PyToolKit(std::string config_file);
    ~PyToolKit();
    void single_process(char *features, int len, PyEntity &entity);
    void cross_process(char *features, int len, PyEntity &entity);
    void process(char *features, int len, PyEntity &entity);
};

#endif // LUBAN_PYLUBAN_H
