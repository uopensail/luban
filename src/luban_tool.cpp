/*
 * 这是批量处理训练数据的工具
 */
#include <getopt.h>
#include <google/protobuf/util/json_util.h>
#include <stdio.h>

#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <thread>

#include "toolkit.h"

void split_file(std::string filename, int num) {
  std::cout << "Reading File: " << filename << std::endl;
  std::ofstream *fds[num];
  for (int i = 0; i < num; i++) {
    fds[i] =
        new std::ofstream(filename + "_" + std::to_string(i), std::ios::out);
  }
  std::ifstream reader(filename, std::ios::in);
  if (!reader) {
    exit(-1);
  }
  int count = 0;
  std::string line;
  while (getline(reader, line)) {
    count++;
    if (count % 1000 == 0) {
      std::cout << "Reading File: " << filename << " Line No." << count
                << std::endl;
    }
    line += "\n";
    fds[rand() % num]->write(line.c_str(), line.size());
  }
  std::cout << "Finish Reading File: " << filename
            << " Total Line Count: " << count << std::endl;
  reader.close();
  for (int i = 0; i < num; i++) {
    fds[i]->close();
  }
}

void write_record(std::ofstream &writer, int label,
                  std::vector<u_int64_t> &ret) {
  writer << label;
  for (size_t i = 0; i < ret.size(); i++) {
    writer << " " << ret[i];
  }
  writer << "\n";
}

int get_label(const sample::Features &features) {
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
}

void merge_file(std::ofstream &writer, std::string filename) {
  std::cout << "Merge File: " << filename << std::endl;
  std::ifstream reader(filename, std::ios::in);
  if (!reader) {
    exit(-1);
  }
  int count = 0;
  std::string line;
  while (getline(reader, line)) {
    count++;
    if (count % 1000 == 0) {
      std::cout << "Reading File: " << filename << " Line No." << count
                << std::endl;
    }
    line += "\n";
    writer.write(line.c_str(), line.size());
  }
  std::cout << "Finish Merging File: " << filename
            << " Total Line Count: " << count << std::endl;
  reader.close();
  remove(filename.c_str());
}

void thread_func(std::string config_file, std::string input_file,
                 std::string output_file) {
  std::cout << "Processing File: " << input_file << std::endl;
  luban::ToolKit toolkit(config_file);
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
      std::cout << "Reading File: " << input_file << " Line No." << count
                << std::endl;
    }
    sample::Example example;
    if (google::protobuf::util::JsonStringToMessage(line, &example, opts)
            .ok()) {
      label = get_label(example.features());
      if (label < 0) {
        continue;
      }
      std::vector<u_int64_t> ret;
      toolkit.process(example.features(), ret);
      if (ret.size() > 0) {
        write_record(writer, label, ret);
      }
    }
  }
  std::cout << "Finish Processing File: " << input_file
            << " Total Line Count: " << count << std::endl;
  reader.close();
  remove(input_file.c_str());
  writer.close();
}

void process(std::string config_file, std::string input_file,
             std::string out_file, int num) {
  split_file(input_file, num);
  std::thread threads[num];
  for (int i = 0; i < num; i++) {
    threads[i] = std::thread(thread_func, config_file,
                             input_file + "_" + std::to_string(i),
                             out_file + "_" + std::to_string(i));
    threads[i].join();
  }

  std::ofstream writer(out_file, std::ios::out);
  if (!writer) {
    exit(-1);
  }
  for (int i = 0; i < num; i++) {
    merge_file(writer, out_file + "_" + std::to_string(i));
  }
  writer.close();
}

int main(int argc, char *argv[]) {
  int opt, num;
  std::string config_file, input_file, out_file;
  while ((opt = getopt(argc, argv, "c:i:o:n:")) != -1) {
    switch (opt) {
    case 'c':
      config_file = std::string(optarg);
      break;
    case 'i':
      input_file = std::string(optarg);
      break;
    case 'o':
      out_file = std::string(optarg);
      break;
    case 'n':
      num = std::atoi(optarg);
    default:
      std::cout << "./luban_tool -c config_file -i input_file -o output_file "
                   "-n thread_num"
                << std::endl;
    }
  }
  process(config_file, input_file, out_file, num);
  return 0;
}
