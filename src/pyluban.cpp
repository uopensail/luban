#include "pyluban.h"

#include "feature.pb.h"
PyEntityArray::PyEntityArray() : data_(nullptr) {}

Entity *PyEntityArray::get(int index) {
  if (data_ == nullptr) {
    return nullptr;
  }
  if ((index >= 0) && (index < data_->size)) {
    return data_->array[index];
  }
  return nullptr;
}

PyEntityArray ::~PyEntityArray() { del_entity_array(data_); }

int PyEntityArray ::size() {
  if (data_ == nullptr) {
    return 0;
  }
  return data_->size;
}

PyToolKit::PyToolKit(std::string config_file) {
  toolkit = new Toolkit(config_file);
}

PyToolKit::~PyToolKit() { delete toolkit; }

void PyToolKit::process(char *features, int len, PyEntityArray &entity) {
  sample::Features *feat = new sample::Features();
  feat->ParseFromArray(features, len);
  entity.data_ = toolkit->process(feat);
  delete feat;
}

void PyToolKit::process_file(std::string input_file, std::string output_file) {}