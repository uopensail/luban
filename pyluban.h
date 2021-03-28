#ifndef LUBAN_PYLUBAN_H
#define LUBAN_PYLUBAN_H

#include <vector>
#include <string>


class PyToolKit {
private:
    void *toolkit;
public:
    PyToolKit(std::string config_file);

    ~PyToolKit();

    std::vector<u_int64_t> single_process(std::string &features);

    std::vector<u_int64_t> cross_process(std::string &features);

    std::vector<u_int64_t> process(std::string &features);

};


#endif //LUBAN_PYLUBAN_H
