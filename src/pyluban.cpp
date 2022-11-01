#include "pyluban.h"

#include "feature.pb.h"
PyEntityArray::PyEntity() : data_(nullptr), size_(0) {}

void data_::init(int size) {
  assert(size > 0);
  data_ = new_entity_array(size);
  size_ = size;
}

Entity *PyEntityArray::get(int index) {
  if ((index >= 0) && (index < size_)) {
    return data_->array[index];
  }
  return nullptr;
}

PyEntityArray ::~PyEntityArray() { del_entity_array(data_); }

int PyEntityArray ::size() { return size_; }

PyToolKit::PyToolKit(std::string config_file) {
  toolkit = new ToolKit(config_file);
}

PyToolKit::~PyToolKit() { delete toolkit; }

void PyToolKit::process(char *features, int len, PyEntityArray &entity) {
  toolkit->process(features, len, &entity.data_);
}

void PyToolKit::process_file(std::string input_file, std::string output_file) {}