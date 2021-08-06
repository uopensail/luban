
#include "utils.h"


u_int64_t luban::mask(u_int64_t &&v, u_int64_t &slot) {
    if (slot >= 0) {
        return (slot << slot_shift) + (slot_mask & v);
    }
    return v;
}

u_int64_t luban::mmh3(std::string &key) {
    u_int64_t ret[2];
    MurmurHash3_x64_128(key.c_str(), key.size(), 0, &ret);
    return ret[0];
}

//分桶
int luban::get_bucket(float &value, float &low, float &high, int &buckets) {
    if (buckets <= 0 || high <= low) {
        return 0;
    }
    if (value >= high) {
        return buckets;
    } else if (value <= low) {
        return 0;
    } else {
        return int((value - low) / (high - low) * buckets);
    }
}

u_int64_t luban::mmh3(std::string &&key) {
    u_int64_t ret[2];
    MurmurHash3_x64_128(key.c_str(), key.size(), 0, &ret);
    return ret[0];
}

//特征转成string
void luban::feature_to_strings(tensorflow::Feature &feature, std::vector<std::string> &list) {
    if (feature.bytes_list().value_size() > 0) {
        auto vlist = feature.bytes_list();
        for (int i = 0; i < vlist.value_size(); i++) {
            list.push_back(vlist.value(i));
        }
        return;
    }

    if (feature.int64_list().value_size() > 0) {
        //整形
        auto vlist = feature.int64_list();
        for (int i = 0; i < vlist.value_size(); i++) {
            list.push_back(std::to_string(vlist.value(i)));
        }
        return;
    }

    if (feature.float_list().value_size() > 0) {
        //浮点型
        auto vlist = feature.float_list();
        for (int i = 0; i < vlist.value_size(); i++) {
            list.push_back(std::to_string(vlist.value(i)));
        }
        return;
    }
}

//特征转成float
void luban::feature_to_floats(tensorflow::Feature &feature, std::vector<float> &list) {
    if (feature.int64_list().value_size() > 0) {
        //整形
        auto vlist = feature.int64_list();
        for (int i = 0; i < vlist.value_size(); i++) {
            list.push_back(float(vlist.value(i)));
        }
        return;
    }
    if (feature.float_list().value_size() > 0) {
        //浮点型
        auto vlist = feature.float_list();
        for (int i = 0; i < vlist.value_size(); i++) {
            list.push_back(vlist.value(i));
        }
        return;
    }
}

u_int64_t luban::get_current_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (u_int64_t) (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

