#include "luban.h"

#include "toolkit.hpp"
void *luban_toolkit_new(const char *config_file) {
  Toolkit *toolkit = new Toolkit(config_file);
  return toolkit;
}
void luban_toolkit_release(void *ptr) {
  if (ptr != nullptr) {
    delete (Toolkit *)ptr;
  }
}

void *luban_process(void *ptr, char *feature, int feature_len);
void *luban_unary_process(void *ptr, char *feature, int feature_len) {
  if (ptr == nullptr || feature == nullptr || feature_len == 0) {
    return nullptr;
  }
  Toolkit *toolkit = (Toolkit *)ptr;
  EntityArray *array = nullptr;
  toolkit->process(feature, feature_len, &array);
  return array;
}

void *luban_binary_process(void *ptr, char *featureA, int feature_lenA,
                           char *featureB, int feature_lenB) {
  if (ptr == nullptr || featureA == nullptr || feature_lenA == 0 ||
      featureB == nullptr || feature_lenB == 0) {
    return nullptr;
  }
  Toolkit *toolkit = (Toolkit *)ptr;
  EntityArray *array = nullptr;
  SharedFeaturesPtr featuresA = std::make_shared<tensorflow::Features>();
  featuresA->ParseFromArray(featureA, feature_lenA);

  SharedFeaturesPtr featuresB = std::make_shared<tensorflow::Features>();
  featuresA->ParseFromArray(featureB, feature_lenB);

  toolkit->process({featuresA, featuresB}, &array);
  return array;
}

void luban_entity_array_release(void *ptr) {
  if (ptr != nullptr) {
    del_entity_array((EntityArray *)ptr);
  }
}