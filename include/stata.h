#ifndef LUBAN_STATA_H
#define LUBAN_STATA_H

#include "json.hpp"
#include <atomic>
#include <chrono>
#include <glog/logging.h>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))

namespace stata
{
    const int capacity = 50000;
    const int interval = 30000;
    enum Status
    {
        Err = 1,
        OK = 2,
        Miss = 3
    };

    void do_stat_func();

    class Unit
    {
    public:
        std::string name_;
        int count_;
        Status status_;
        __int64_t start_;
        __int64_t end_;

    public:
        Unit() = delete;

        Unit(const Unit &);

        Unit(const Unit &&);

        Unit(std::string &name);

        Unit(std::string &&name);

        Unit(const char *name);

        ~Unit();

        Unit &MarkOK();

        Unit &MarkErr();

        Unit &MarkMiss();

        Unit &SetCount(int count);

        void End();
    };

    class Info
    {
    public:
        std::string name_;
        Status status_;
        int total_;
        float qps_;
        int total_count_;
        float avg_count_;
        float avg_cost_;
        float max_cost_;
        __int64_t total_cost_;

    public:
        Info() = delete;

        Info(const Info &) = delete;

        Info(const Info &&) = delete;

        Info(std::string &name, Status &status);

        ~Info();
    };

    class Stata
    {
    private:
        std::atomic<__int64_t> add_index_;
        __int64_t stat_index_;
        std::string stat_info_;
        std::thread stat_thread_;
        std::vector<Unit *> buffer_;
        static Stata *global_stata_;

    private:
        Stata();

        ~Stata();

    public:
        Stata(const Stata &) = delete;

        Stata(const Stata &&) = delete;

        const Stata &operator=(const Stata &) = delete;

        const Stata &operator=(const Stata &&) = delete;

        static Stata *getStata();

        static void deleteStata();

        void add(Unit &u);

        void do_stat();

        std::string &get_status();
    };
} // namespace stata
#endif // LUBAN_STATA_H
