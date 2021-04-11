#ifndef __LUBAN_BOX_HELPER_H__
#define __LUBAN_BOX_HELPER_H__

#include "cpptoml.h"
#include "feature.pb.h"

namespace luban {
    class ParamsHelper {
    private:
        std::shared_ptr<cpptoml::table> param_table_ptr;

    public:
        ParamsHelper() = delete;

        ~ParamsHelper() {}

        ParamsHelper(const std::shared_ptr<cpptoml::table> &table) : param_table_ptr(table) {}

        ParamsHelper(const ParamsHelper &param) : param_table_ptr(param.param_table_ptr) {}

        ParamsHelper &operator=(const ParamsHelper &param) {
            param_table_ptr = param.param_table_ptr;
            return *this;
        }

        template<typename T>
        T get(const std::string &key) {
            if (param_table_ptr->contains(key)) {
                return *(param_table_ptr->get_as<T>(key));
            } else {
                throw std::out_of_range{key + " is not a valid key"};
            }
        }
    };

    class FeaturesHelper {
    private:
        std::shared_ptr<tensorflow::Features> features_ptr;

    public:
        FeaturesHelper() = delete;

        ~FeaturesHelper() {}

        FeaturesHelper(tensorflow::Features *features) : features_ptr(features) {}

        FeaturesHelper(const FeaturesHelper &features) : features_ptr(features.features_ptr) {}

        FeaturesHelper &operator=(const FeaturesHelper &features) {
            features_ptr = features.features_ptr;
            return *this;
        }

        FeaturesHelper(const char *features, int len) {
            auto ptr = new tensorflow::Features();
            ptr->ParseFromArray(features, len);
            features_ptr.reset(ptr);
        }

        tensorflow::Feature &&get(const std::string &key) {
            try {
                auto feature = features_ptr->feature();
                auto iter = feature.find(key);
                if (iter != feature.end()) {
                    return std::move(iter->second);
                } else {
                    throw std::out_of_range{key + " is not a valid key"};
                }
            }
            catch (...) {
                throw std::out_of_range{key + " is not a valid key"};
            }
        }
    };
} // namespace luban

#endif //__LUBAN_BOX_HELPER_H__