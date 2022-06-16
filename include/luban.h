#ifndef LUBAN_LUBAN_H
#define LUBAN_LUBAN_H

#include "feature.pb.h"
#include <string>

class LuBan
{
private:
    void *toolkit;

public:
    LuBan(std::string config_file);

    ~LuBan();

    void process(tensorflow::Features &features, std::vector<u_int64_t> &ret);

    void single_process(tensorflow::Features &features, std::vector<u_int64_t> &ret);

    void bicross_process(tensorflow::Features &featuresA,
                         tensorflow::Features &featuresB, std::vector<u_int64_t> &ret);
};

#endif // LUBAN_LUBAN_H
