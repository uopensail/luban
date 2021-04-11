#include "pyluban.h"
#include "feature.pb.h"

PyToolKit::PyToolKit(std::string config_file) {
    toolkit = new luban::ToolKit(config_file);
}

PyToolKit::~PyToolKit() {
    delete toolkit;
}

std::vector<u_int64_t> PyToolKit::single_process(std::string &feature_str) {
    tensorflow::Features features;
    features.ParseFromString(feature_str);
    std::vector<u_int64_t> ret;
    toolkit->single_process(features, ret);
    return ret;
}

std::vector<u_int64_t> PyToolKit::cross_process(std::string &feature_str) {
    tensorflow::Features features;
    features.ParseFromString(feature_str);
    std::vector<u_int64_t> ret;
    toolkit->cross_process(features, ret);
    return ret;
}

std::vector<u_int64_t> PyToolKit::process(std::string &feature_str) {
    tensorflow::Features features;
    features.ParseFromString(feature_str);
    std::vector<u_int64_t> ret;
    toolkit->process(features, ret);
    return ret;
}