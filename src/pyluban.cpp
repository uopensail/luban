#include "pyluban.h"

#include <google/protobuf/util/json_util.h>

#include <fstream>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

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

void PyToolKit::process_file(std::string input_file, std::string output_file) {
  // split the file
  auto split_file = [](std::string filename, int num) {
    std::cout << "reading file: " << filename << std::endl;
    std::ifstream reader(filename, std::ios::in);
    if (!reader) {
      exit(1);
    }
    std::ofstream **fds = new std::ofstream *[num];
    for (int i = 0; i < num; i++) {
      fds[i] =
          new std::ofstream(filename + "_" + std::to_string(i), std::ios::out);
    }
    size_t count = 0;
    std::string line;
    int index = 0;
    while (std::getline(reader, line)) {
      count++;
      std::cout << "reading file: " << filename << "line NO.: " << count
                << std::endl;
      index = rand() % num;
      fds[index]->write(line.c_str(), line.size());
      fds[index]->write("\n", 1);
    }
    std::cout << "finish reading file: " << filename << "total line count"
              << count << std::endl;
    reader.close();
    for (int i = 0; i < num; i++) {
      fds[i]->close();
    }
    delete[] fds;
  };

  auto write_record = [](std::ofstream &writer, int label, EntityArray *data) {
    writer << label;
    for (size_t i = 0; i < data->size; i++) {
      if (data->array[i] != nullptr) {
        for (size_t j = 0; j < data->array[i]->size; j++) {
          writer << " " << data->array[i]->data[j];
        }
      }
    }
    writer << "\n";
  };

  auto get_label = [](const sample::Features &features) -> int {
    auto features_map = features.feature();
    auto iter = features_map.find("label");
    if (iter == features_map.end()) {
      return -1;
    }
    auto list = iter->second.int64_list();

    if (list.value_size() > 0) {
      return int(list.value(0));
    }
    return -1;
  };

  auto merge_file = [](std::ofstream &writer, std::string filename) {
    std::cout << "merge file: " << filename << std::endl;
    std::ifstream reader(filename, std::ios::in);
    if (!reader) {
      exit(-1);
    }
    int count = 0;
    std::string line;
    while (getline(reader, line)) {
      count++;
      if (count % 1000 == 0) {
        std::cout << "reading file: " << filename << " line NO." << count
                  << std::endl;
      }
      line += "\n";
      writer.write(line.c_str(), line.size());
    }
    std::cout << "finish merging file: " << filename
              << " total line count: " << count << std::endl;
    reader.close();
    remove(filename.c_str());
  };

  auto thread_func = [&](Toolkit *toolkit, std::string input_file,
                         std::string output_file) {
    std::cout << "processing file: " << input_file << std::endl;

    google::protobuf::util::JsonParseOptions opts;
    opts.ignore_unknown_fields = true;
    std::ofstream writer(output_file, std::ios::out);
    if (!writer) {
      exit(-1);
    }
    std::ifstream reader(input_file, std::ios::in);
    if (!reader) {
      exit(-1);
    }
    int count = 0, label = -1;
    std::string line;
    while (getline(reader, line)) {
      count++;
      if (count % 1000 == 0) {
        std::cout << "reading file: " << input_file << " line NO." << count
                  << std::endl;
      }
      sample::Example example;
      if (google::protobuf::util::JsonStringToMessage(line, &example, opts)
              .ok()) {
        const sample::Features &features = example.features();
        label = get_label(features);
        auto array = toolkit->process((sample::Features *)&features);
        write_record(writer, label, array);
        del_entity_array(array);
      }
    }
    std::cout << "finish processing file: " << input_file
              << " total line count: " << count << std::endl;
    reader.close();
    remove(input_file.c_str());
    writer.close();
  };

  int num = std::max<int>(1, std::thread::hardware_concurrency() - 1);
  split_file(input_file, num);
  std::vector<std::thread> threads;
  threads.reserve(num);
  for (int i = 0; i < num; i++) {
    threads.at(i) =
        std::thread(thread_func, toolkit, input_file + "_" + std::to_string(i),
                    output_file + "_" + std::to_string(i));
    threads[i].join();
  }

  std::ofstream writer(output_file, std::ios::out);
  if (!writer) {
    exit(-1);
  }
  for (int i = 0; i < num; i++) {
    merge_file(writer, output_file + "_" + std::to_string(i));
  }
  writer.close();
}