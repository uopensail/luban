#ifndef LUBAN_UTILS_H
#define LUBAN_UTILS_H

#pragma once

#include "MurmurHash3.h"
#include "feature.pb.h"
#include "helper.h"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <sys/time.h>
#include <time.h>
#include <typeinfo>
#include <vector>

//判断类型
#define is_int(T)                                                              \
  (std::is_same<T, long long>::value || std::is_same<T, int>::value ||         \
   std::is_same<T, long>::value || std::is_same<T, unsigned long long>::value)
#define is_float(T)                                                            \
  (std::is_same<T, float>::value || std::is_same<T, double>::value)
#define is_str(T) (std::is_same<T, std::string>::value)

//字符串split
void split(const std::string &s, std::vector<std::string> &tokens,
           const std::string &delimiters = " ");

//获得当前的时间戳
u_int64_t get_current_time();

void split_file(std::string filename, int num);
void write_record(std::ofstream &writer, int label,
                  std::vector<u_int64_t> &ret);
int get_label(const tensorflow::Features &features);
#endif // LUBAN_UTILS_H