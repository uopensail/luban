#include "pyluban.h"
#include "feature.pb.h"

PyToolKit::PyToolKit(std::string config_file) {
    toolkit = new luban::ToolKit(config_file);
}

PyToolKit::~PyToolKit() {
    delete toolkit;
}

std::vector<unsigned long long> PyToolKit::single_process(char *features, int len) {
    tensorflow::Features tf_features;
    tf_features.ParseFromArray(features, len);
    std::vector<unsigned long long> ret;
    toolkit->single_process(tf_features, ret);
    return ret;
}

std::vector<unsigned long long> PyToolKit::cross_process(char *features, int len) {
    tensorflow::Features tf_features;
    tf_features.ParseFromArray(features, len);
    std::vector<unsigned long long> ret;
    toolkit->cross_process(tf_features, ret);
    return ret;
}

std::vector<unsigned long long> PyToolKit::process(char *features, int len) {
    tensorflow::Features tf_features;
    tf_features.ParseFromArray(features, len);
    std::vector<unsigned long long> ret;
    toolkit->process(tf_features, ret);
    return ret;
}