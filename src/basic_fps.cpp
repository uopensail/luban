#include "basic_fps.h"


void luban::kv(std::string &key, tensorflow::Feature &feature, ParamsHelper &params, std::vector<u_int64_t> &ret) {
    auto slot = params.get<u_int64_t>("slot_id");
    std::vector<std::string> list;
    feature_to_strings(feature, list);
    for (auto &v:list) {
        ret.push_back(mask(mmh3("kv|" + key + ":" + v), slot));
    }
}

void luban::raw(std::string &key, tensorflow::Feature &feature, ParamsHelper &params, std::vector<u_int64_t> &ret) {
    auto slot = params.get<u_int64_t>("slot_id");
    std::vector<std::string> list;
    feature_to_strings(feature, list);
    for (auto &v:list) {
        ret.push_back(mask(mmh3(v), slot));
    }
}

void luban::bucket(std::string &key, tensorflow::Feature &feature, ParamsHelper &params,
                   std::vector<u_int64_t> &ret) {
    auto slot = params.get<u_int64_t>("slot_id");
    float low = params.get<double>("low");
    float high = params.get<double>("high");
    int buckets = params.get<int>("buckets");
    std::vector<float> list;
    feature_to_floats(feature, list);
    for (auto &v:list) {
        ret.push_back(mask(mmh3("bucket|" + key + ":" + std::to_string(get_bucket(v, low, high, buckets))), slot));
    }
}

void luban::log_int(std::string &key, tensorflow::Feature &feature,
                    ParamsHelper &params, std::vector<u_int64_t> &ret) {
    auto slot = (u_int64_t) params.get<int>("slot_id");
    std::vector<float> list;
    feature_to_floats(feature, list);
    for (auto &v:list) {
        if (v <= 0) {
            continue;
        }
        ret.push_back(mask(mmh3("log_int|" + key + ":" + std::to_string(int(log(v)))), slot));
    }
    return;
}

void luban::log_bucket(std::string &key, tensorflow::Feature &feature,
                       ParamsHelper &params, std::vector<u_int64_t> &ret) {
    auto slot = (u_int64_t) params.get<int>("slot_id");
    float low = params.get<double>("low");
    float high = params.get<double>("high");
    int buckets = params.get<int>("buckets");
    std::vector<float> list;
    feature_to_floats(feature, list);
    float tmp;
    for (auto &v:list) {
        if (v <= 0) {
            continue;
        }
        tmp = log(v);
        ret.push_back(
                mask(mmh3("log_bucket|" + key + std::to_string(get_bucket(tmp, low, high, buckets))), slot));
    }
    return;
}

void luban::hour_diff(std::string &key, tensorflow::Feature &feature,
                      ParamsHelper &params, std::vector<u_int64_t> &ret) {
    auto slot = (u_int64_t) params.get<int>("slot_id");
    auto tm = get_current_time();
    std::vector<float> list;
    feature_to_floats(feature, list);
    int diff;
    //ms
    for (auto &v:list) {
        diff = int((tm - v) / 3600000.0);
        ret.push_back(mask(mmh3("hour_diff|" + key + ":" + std::to_string(diff)), slot));
    }
    return;
}

void luban::merge(std::string &keyA, tensorflow::Feature &featureA,
                  std::string &keyB, tensorflow::Feature &featureB,
                  ParamsHelper &params, std::vector<u_int64_t> &ret) {
    auto slot = (u_int64_t) params.get<int>("slot_id");
    std::vector<std::string> listA, listB;
    feature_to_strings(featureA, listA);
    feature_to_strings(featureB, listB);
    for (auto &vA:listA) {
        for (auto &vB:listB) {
            ret.push_back(mask(mmh3("merge|" + keyA + ":" + vA + "|" + keyB + ":" + vB), slot));
        }
    }
    return;
}

void luban::co_occur(std::string &keyA, tensorflow::Feature &featureA,
                     std::string &keyB, tensorflow::Feature &featureB,
                     ParamsHelper &params, std::vector<u_int64_t> &ret) {
    auto slot = (u_int64_t) params.get<int>("slot_id");
    std::vector<std::string> listA, listB;
    feature_to_strings(featureA, listA);
    feature_to_strings(featureB, listB);
    if (listA.empty() || listB.empty()) {
        return;
    }
    std::map<std::string, int> cache;
    for (auto &vA:listA) {
        cache[vA] = 1;
    }

    int num = 0;
    for (auto &vB:listB) {
        if (cache.find(vB) != cache.end()) {
            num++;
        }
    }
    ret.push_back(mask(mmh3("co_occur|" + keyA + "|" + keyB + ":" + std::to_string(num)), slot));
    return;
}



