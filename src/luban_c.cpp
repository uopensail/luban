#include "luban_c.h"
#include "toolkit.h"

void *luban_new(char *config_file) {
    std::string config(config_file);
    return new luban::ToolKit(config);
}

void luban_release(void *ptr) {
    delete (luban::ToolKit *) ptr;
}

void *luban_process(void *ptr, char *feature, int feature_len, void *return_len) {
    auto toolkit = (luban::ToolKit *) ptr;
    tensorflow::Features features;
    features.ParseFromArray(feature, feature_len);
    std::vector<u_int64_t> ret;
    toolkit->process(features, ret);
    if (!ret.empty()) {
        u_int64_t *ans = (u_int64_t *) malloc(sizeof(u_int64_t) * ret.size());
        for (int i = 0; i < ret.size(); i++) {
            ans[i] = ret[i];
        }
        return ans;
    }
    return nullptr;
}

void *luban_single_process(void *ptr, char *feature, int feature_len, void *return_len) {
    auto toolkit = (luban::ToolKit *) ptr;
    tensorflow::Features features;
    features.ParseFromArray(feature, feature_len);
    std::vector<u_int64_t> ret;
    toolkit->single_process(features, ret);
    if (!ret.empty()) {
        u_int64_t *ans = (u_int64_t *) malloc(sizeof(u_int64_t) * ret.size());
        for (int i = 0; i < ret.size(); i++) {
            ans[i] = ret[i];
        }
        return ans;
    }
    return nullptr;
}

void *luban_bicross_process(void *ptr, char *featureA, int feature_lenA,
                            char *featureB, int feature_lenB, void *return_len) {
    auto toolkit = (luban::ToolKit *) ptr;
    tensorflow::Features featuresA, featuresB;
    featuresA.ParseFromArray(featureA, feature_lenA);
    featuresB.ParseFromArray(featureB, feature_lenB);
    std::vector<u_int64_t> ret;
    toolkit->bicross_process(featuresA, featuresB, ret);
    if (!ret.empty()) {
        u_int64_t *ans = (u_int64_t *) malloc(sizeof(u_int64_t) * ret.size());
        for (int i = 0; i < ret.size(); i++) {
            ans[i] = ret[i];
        }
        return ans;
    }
    return nullptr;
}