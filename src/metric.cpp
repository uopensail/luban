#include "metric.h"
#include <iostream>
std::string *metric::print_metric(const Metric &m)
{
    std::string *ret = new std::string();
    ret->append("{\"name\":\"");
    ret->append(m.name);
    ret->append("\",\"total\":");
    ret->append(std::to_string(m.total_num));
    ret->append(",\"qps\":");
    ret->append(std::to_string(m.qps * 1000));
    ret->append(",\"counter\":");
    ret->append(std::to_string(m.total_count));
    ret->append(",\"avg_counter\":");
    ret->append(std::to_string(m.avg_count));
    ret->append(",\"avg_cost\":");
    ret->append(std::to_string(m.avg_cost));
    ret->append(",\"max_cost\":");
    ret->append(std::to_string(m.max_cost));
    ret->append(",\"p95\":");
    ret->append(std::to_string(m.p95_cost));
    ret->append(",\"p99\":");
    ret->append(std::to_string(m.p99_cost));
    switch (m.status)
    {
    case Status::Err:
        ret->append(",\"status\":\"Err\"}");
        break;
    case Status::OK:
        ret->append(",\"status\":\"OK\"}");
        break;
    case Status::Miss:
        ret->append(",\"status\":\"Miss\"}");
        break;
    default:
        ret->append("}");
        break;
    }
    return ret;
}

int metric::cmp(const Unit *a, const Unit *b)
{
    return a->cost < b->cost ? 1 : (a->cost == b->cost ? 0 : -1);
}

metric::Unit::Unit() : status(Status::OK),
                       _isAdd(false),
                       count(0),
                       start(std::chrono::duration_cast<std::chrono::microseconds>(
                                 std::chrono::steady_clock::now().time_since_epoch())
                                 .count()),
                       end(start),
                       cost(0),
                       name("")
{
}

metric::Unit::Unit(const char *name) : status(Status::OK),
                                       _isAdd(false),
                                       count(0),
                                       start(std::chrono::duration_cast<std::chrono::microseconds>(
                                                 std::chrono::steady_clock::now().time_since_epoch())
                                                 .count()),
                                       end(start),
                                       cost(0),
                                       name(name)
{
}

metric::Unit &metric::Unit::operator=(const Unit &u)
{
    status = u.status;
    _isAdd = u._isAdd;
    count = u.count;
    start = u.start;
    end = u.end;
    cost = u.cost;
    name = u.name;
    return *this;
}

metric::Unit::Unit(const std::string &name) : Unit(name.c_str())
{
}

metric::Unit::Unit(const std::string &&name) : Unit(name.c_str()) {}

metric::Unit::~Unit()
{
    if (!_isAdd)
    {
        End();
    }
}

metric::Unit::Unit(const Unit &u) : status(u.status),
                                    _isAdd(u._isAdd),
                                    count(u.count),
                                    start(u.start),
                                    end(u.end),
                                    name(u.name)
{
}

metric::Unit &metric::Unit::MarkOK()
{
    this->status = Status::OK;
    return *this;
}

metric::Unit &metric::Unit::MarkErr()
{
    this->status = Status::Err;
    return *this;
}

metric::Unit &metric::Unit::MarkMiss()
{
    this->status = Status::Miss;
    return *this;
}

metric::Unit &metric::Unit::SetCount(__int64_t count)
{
    this->count = count;
    return *this;
}

void metric::Unit::End()
{
    end = std::chrono::duration_cast<std::chrono::microseconds>(
              std::chrono::steady_clock::now().time_since_epoch())
              .count();
    cost = end - start;
    __int64_t index = global_insert_index.fetch_add(1);
    __int64_t idx = index % capacity;
    _isAdd = true;
    global_unit_pool[idx] = *this;
}

//统计每一个子项
std::string *metric::sub_statistic(const std::string name, Status status, UnitVector *data)
{
    // data里面的数据从小到大按照cost进行排序
    if (data == nullptr)
    {
        return nullptr;
    }
    std::sort(data->begin(), data->end(), cmp);
    Metric m;
    m.status = status;
    m.name = name;
    size_t size = data->size();
    m.p99_cost = data->at(size_t(double(size) * 0.99))->cost;
    m.p95_cost = data->at(size_t(double(size) * 0.95))->cost;
    m.total_num = size;

    __int64_t start = std::chrono::duration_cast<std::chrono::microseconds>(
                          std::chrono::steady_clock::now().time_since_epoch())
                          .count(),
              end = 0, time_diff = 0;
    Unit *ptr = nullptr;
    for (size_t i = 0; i < size; i++)
    {
        ptr = data->at(i);
        m.total_count += ptr->count;
        m.total_cost += ptr->cost;
        m.max_cost = max(m.max_cost, ptr->cost);
        start = min(start, ptr->start);
        end = max(end, ptr->end);
    }
    m.avg_cost = float(m.total_cost) / float(m.total_num);
    m.avg_count = float(m.total_count) / float(m.total_num);
    time_diff = end - start;

    if (time_diff > 0)
    {
        m.qps = float(m.total_num) / time_diff;
        return print_metric(m);
    }
    return nullptr;
}

void metric::do_statistic(Unit src[], Unit dst[], __int64_t start_index, __int64_t end_index)
{
    //存放各个状态的结果
    UnitPool okPool, errPool, missPool;
    //添加到pool中去
    auto add_to_pool = [&okPool, &errPool, &missPool](Unit *unit)
    {
        UnitPool *pool = nullptr;
        switch (unit->status)
        {
        case Status::OK:
            pool = &okPool;
            break;
        case Status::Err:
            pool = &errPool;
            break;
        case Status::Miss:
            pool = &missPool;
            break;
        default:
            break;
        }
        if (pool == nullptr)
        {
            return;
        }
        auto iter = pool->find(unit->name);
        if (iter == pool->end())
        {
            auto vec = new UnitVector();
            vec->push_back(unit);
            (*pool)[unit->name] = vec;
        }
        else
        {
            iter->second->push_back(unit);
        }
    };
    //数据拷贝
    for (__int64_t i = start_index; i < end_index; i++)
    {
        dst[i - start_index] = src[i % capacity];
    }
    auto get_information = [](Status status, UnitPool *pool) -> std::string *
    {
        size_t index = 0;
        std::string *ret = new std::string();
        for (auto &iter : *pool)
        {
            auto info = sub_statistic(iter.first, status, iter.second);
            if (info == nullptr)
            {
                continue;
            }
            if (index > 0)
            {
                ret->append(",");
            }
            ret->append(*info);
            index++;
            delete info;
            delete iter.second;
        }
        pool->clear();
        if (ret->size() == 0)
        {
            delete ret;
            return nullptr;
        }
        return ret;
    };

    std::string *information = new std::string();
    auto ok_information = get_information(Status::OK, &okPool);
    auto err_information = get_information(Status::Err, &errPool);
    auto miss_information = get_information(Status::Miss, &missPool);
    information->append("[");
    bool isfirst = true;
    if (ok_information != nullptr)
    {
        if (!isfirst)
        {
            information->append(",");
            isfirst = false;
        }
        information->append(*ok_information);
        delete ok_information;
    }

    if (err_information != nullptr)
    {
        if (!isfirst)
        {
            information->append(",");
            isfirst = false;
        }
        information->append(*err_information);
        delete err_information;
    }
    if (miss_information != nullptr)
    {
        if (!isfirst)
        {
            information->append(",");
            isfirst = false;
        }
        information->append(*miss_information);
        delete miss_information;
    }

    information->append("]");

    std::string *origin_information = global_information;
    global_information = information;
    delete origin_information;
}

void metric::start()
{
    LOG(INFO) << "metric start";

    std::cout << "metric 0";
    
    global_thread_status.store(true);
    std::cout << "metric 1";
    global_information = new std::string("[]");
    std::cout << "metric 2";
    global_metric_thread = new std::thread(do_metric);
    std::cout << "metric xxxxx";
    global_metric_thread->detach();
}

void metric::stop()
{
    global_thread_status.store(false);
    std::this_thread::sleep_for(std::chrono::milliseconds(interval));
    LOG(INFO) << "metric stop";
    delete global_metric_thread;
}

void metric::do_metric()
{
    Unit buffer[capacity];
    for (; global_thread_status.load();)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        auto end_index = global_insert_index.load();
        auto start_index = global_metric_index.load();

        auto diff = end_index - start_index;
        if (diff >= capacity)
        {
            start_index = end_index - capacity + 1;
            global_metric_index.store(start_index);
        }
        do_statistic(global_unit_pool, buffer, start_index, end_index);
        LOG(INFO) << *global_information;
    }
}
