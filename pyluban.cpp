#include "pyluban.h"
#include "feature.pb.h"
#include "toolkit.h"

PyToolKit::PyToolKit(std::string config_file) {
    toolkit = new luban::ToolKit(config_file);
}

PyToolKit::~PyToolKit() {
    delete (luban::ToolKit *) toolkit;
}

std::vector<u_int64_t> &&PyToolKit::single_process(std::string &feature_str) {
    tensorflow::Features features;
    features.ParseFromString(feature_str);
    return (luban::ToolKit *) toolkit->single_process(features);
}

std::vector<u_int64_t> &&PyToolKit::cross_process(std::string &feature_str) {
    tensorflow::Features features;
    features.ParseFromString(feature_str);
    return (luban::ToolKit *) toolkit->cross_process(features);
}

std::vector<u_int64_t> &&PyToolKit::process(std::string &feature_str) {
    tensorflow::Features features;
    features.ParseFromString(feature_str);
    return (luban::ToolKit *) toolkit->process(features);
}