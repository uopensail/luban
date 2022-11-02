#include "pyluban.h"

#include "feature.pb.h"
PyEntityArray::PyEntityArray() : data_(nullptr), size_(0) {}

Entity *PyEntityArray::get(int index) {
  if ((index >= 0) && (index < size_)) {
    return data_->array[index];
  }
  return nullptr;
}

PyEntityArray ::~PyEntityArray() { del_entity_array(data_); }

int PyEntityArray ::size() { return size_; }

PyToolKit::PyToolKit(std::string config_file) {
  toolkit = new Toolkit(config_file);
}

PyToolKit::~PyToolKit() { delete toolkit; }

void PyToolKit::process(char *features, int len, PyEntityArray &entity) {
  toolkit->process(features, len, &entity.data_);
  entity.size_ = entity.data_->size;
}

void PyToolKit::process_file(std::string input_file, std::string output_file) {}