#ifndef LUBAN_PYLUBAN_H
#define LUBAN_PYLUBAN_H

#include "toolkit.h"
#include <string>
#include <vector>

class PyToolKit;

class PyEntity
{
public:
    Entity **data_;
    int size_;
    friend PyToolKit;

private:
    void init(int size);

public:
    PyEntity();
    ~PyEntity();
    int size();
     Entity* get(int index);
    std::vector<float> get_data(int index);
    std::vector<unsigned long long> get_index(int index);
};

class PyToolKit
{
private:
    ToolKit *toolkit;

public:
    PyToolKit() = delete;
    PyToolKit(std::string config_file);

    ~PyToolKit();

    void single_process(char *features, int len, PyEntity &entity);

    void cross_process(char *features, int len, PyEntity &entity);

    void process(char *features, int len, PyEntity &entity);
};

#endif // LUBAN_PYLUBAN_H
