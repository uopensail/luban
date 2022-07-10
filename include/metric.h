#ifndef LUBAN_METRIC_H
#define LUBAN_METRIC_H
#pragma once
#include <algorithm>
#include <atomic>
#include <chrono>
#include <glog/logging.h>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))

namespace metric
{
    const __int64_t capacity = 5000000; // buffer的大小
    const int interval = 30000;         //单位是ms

    enum Status : int8_t
    {
        Err = 1,
        OK = 2,
        Miss = 3,
        NotSet = 0
    };
    //定义一个统计的单元
    class Unit
    {
    public:
        Status status;
        bool _isAdd;
        __int64_t count;
        __int64_t start;
        __int64_t end;
        __int64_t cost;
        std::string name;

    public:
        Unit();
        Unit(const char *name);
        Unit(const std::string &uname);
        Unit(const std::string &&uname);
        Unit(const Unit &u);
        Unit &operator=(const Unit &);
        Unit &MarkMiss();
        Unit &MarkOK();
        Unit &MarkErr();
        void End();
        Unit &SetCount(__int64_t count);
        ~Unit();
    };

    //统计的值
    struct Metric
    {
        Status status;
        __int64_t total_num;
        __int64_t total_count;
        __int64_t total_cost;
        float qps;
        float avg_count;
        float avg_cost;
        float max_cost;
        float p95_cost;
        float p99_cost;
        std::string name;
    };

    using UnitVector = std::vector<Unit *>;
    using UnitPool = std::unordered_map<std::string, UnitVector *>;
    static int cmp(const Unit *a, const Unit *b);      //按照cost排序
    static std::atomic<__int64_t> global_insert_index; //全局下标
    static std::atomic<__int64_t> global_metric_index; //目前统计的下标
    static Unit global_unit_pool[capacity];            //统计unit的池子
    static std::thread *global_metric_thread;          //统计的线程
    static std::atomic<bool> global_thread_status;     //全局运行状态
    static std::string *global_information;            //全局信息
    std::string *sub_statistic(const std::string, Status status, UnitVector *data);
    void do_statistic(Unit src[], Unit dst[], __int64_t start_index, __int64_t end_index);
    std::string *print_metric(const Metric &m);
    void do_metric();
    void start();
    void stop();
} // namespace metric
#endif // LUBAN_METRIC_H
