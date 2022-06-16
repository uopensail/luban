#ifndef LUBAN_PYLUBAN_H
#define LUBAN_PYLUBAN_H

#include "toolkit.h"
#include <string>
#include <vector>

class PyToolKit
{
private:
    luban::ToolKit *toolkit;

public:
    PyToolKit() = delete;
    PyToolKit(std::string config_file);

    ~PyToolKit();

    std::vector<unsigned long long> single_process(char *features, int len);

    std::vector<unsigned long long> cross_process(char *features, int len);

    std::vector<unsigned long long> process(char *features, int len);
};

#endif // LUBAN_PYLUBAN_H
