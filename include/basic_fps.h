#ifndef LUBAN_BASIC_FPS_H
#define LUBAN_BASIC_FPS_H

#include <math.h>
#include "common.h"
#include "helper.h"
#include "utils.h"

namespace luban {
    void kv(std::string &key, tensorflow::Feature &feature, ParamsHelper &params, std::vector<u_int64_t> &ret);

    void raw(std::string &key, tensorflow::Feature &feature, ParamsHelper &params, std::vector<u_int64_t> &ret);

    void bucket(std::string &key, tensorflow::Feature &feature, ParamsHelper &params,
                std::vector<u_int64_t> &ret);

    void log_int(std::string &key, tensorflow::Feature &feature,
                 ParamsHelper &params, std::vector<u_int64_t> &ret);

    void log_bucket(std::string &key, tensorflow::Feature &feature,
                    ParamsHelper &params, std::vector<u_int64_t> &ret);

    void hour_diff(std::string &key, tensorflow::Feature &feature,
                   ParamsHelper &params, std::vector<u_int64_t> &ret);

    void merge(std::string &keyA, tensorflow::Feature &featureA,
               std::string &keyB, tensorflow::Feature &featureB,
               ParamsHelper &params, std::vector<u_int64_t> &ret);

    void co_occur(std::string &keyA, tensorflow::Feature &featureA,
                  std::string &keyB, tensorflow::Feature &featureB,
                  ParamsHelper &params, std::vector<u_int64_t> &ret);
}

#endif //LUBAN_BASIC_FPS_H
