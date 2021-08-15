#include "stata.h"

stata::Unit::Unit(const Unit &u) : name_(u.name_),
                                   count_(u.count_),
                                   status_(u.status_),
                                   start_(u.start_),
                                   end_(u.end_) {}

stata::Unit::Unit(const Unit &&u) : name_(u.name_),
                                    count_(u.count_),
                                    status_(u.status_),
                                    start_(u.start_),
                                    end_(u.end_) {}

stata::Unit::Unit(std::string &name) : name_(name),
                                       count_(0),
                                       status_(stata::Status::OK),
                                       start_(std::chrono::duration_cast<std::chrono::microseconds>(
                                               std::chrono::steady_clock::now().time_since_epoch()).count()),
                                       end_(start_) {}

stata::Unit::Unit(std::string &&name) : name_(name),
                                        count_(0),
                                        status_(stata::Status::OK),
                                        start_(std::chrono::duration_cast<std::chrono::microseconds>(
                                                std::chrono::steady_clock::now().time_since_epoch()).count()),
                                        end_(start_) {}

stata::Unit::Unit(const char *name) : name_(name),
                                      count_(0),
                                      status_(stata::Status::OK),
                                      start_(std::chrono::duration_cast<std::chrono::microseconds>(
                                              std::chrono::steady_clock::now().time_since_epoch()).count()),
                                      end_(start_) {}

stata::Unit::~Unit() {}

stata::Unit &stata::Unit::MarkOK() {
    status_ = stata::Status::OK;
    return *this;
}

stata::Unit &stata::Unit::MarkErr() {
    status_ = stata::Status::Err;
    return *this;
}

stata::Unit &stata::Unit::SetCount(int count) {
    count_ = count;
    return *this;
}

void stata::Unit::End() {
    end_ = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count();
    stata::Stata::getStata()->add(*this);
}

stata::Info::Info(std::string &name, Status &status) : name_(name),
                                                       status_(status),
                                                       total_(0),
                                                       qps_(0.0),
                                                       total_count_(0),
                                                       avg_count_(0.0),
                                                       avg_cost_(0.0),
                                                       max_cost_(0.0),
                                                       total_cost_(0.0) {}

stata::Info::~Info() {}

stata::Stata::Stata() : add_index_(0),
                        stat_index_(0),
                        stat_info_(""),
                        stat_thread_(stata::do_stat_func) {
    buffer_.resize(stata::capacity, nullptr);
    stat_thread_.detach();
}

stata::Stata::~Stata() {
    stata::Stata::deleteStata();
}

stata::Stata *stata::Stata::getStata() {
    return stata::Stata::global_stata_;
}

void stata::Stata::deleteStata() {
    if (stata::Stata::global_stata_ != nullptr) {
        delete stata::Stata::global_stata_;
        stata::Stata::global_stata_ = nullptr;
    }
}

void stata::Stata::add(Unit &u) {
    Unit *nu = new Unit(u), *ptr;
    auto index = add_index_.fetch_add(1);
    auto idx = index % stata::capacity;
    ptr = buffer_[idx];
    buffer_[idx] = nu;
    if (ptr != nullptr) {
        delete ptr;
    }
}

void stata::Stata::do_stat() {
    Unit *ptr;
    std::unordered_map<std::string, stata::Info *> ok;
    std::unordered_map<std::string, stata::Info *> err;
    stata::Info *info;
    auto end_index = add_index_.load();
    auto diff = end_index - stat_index_;
    if (diff >= stata::capacity) {
        stat_index_ = end_index - stata::capacity + 1;
    }

    __int64_t start = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count(), end = 0, tm = 0;
    for (; stat_index_ < end_index; stat_index_++) {
        ptr = buffer_[stat_index_ % capacity];
        if (ptr == nullptr) {
            continue;
        }

        if (ptr->status_ == stata::Status::OK) {
            auto iter = ok.find(ptr->name_);
            if (iter == ok.end()) {
                info = new Info(ptr->name_, ptr->status_);
                ok[ptr->name_] = info;
            } else {
                info = iter->second;
            }
        } else {
            auto iter = err.find(ptr->name_);
            if (iter == err.end()) {
                info = new Info(ptr->name_, ptr->status_);
                err[ptr->name_] = info;
            } else {
                info = iter->second;
            }
        }

        tm = ptr->end_ - ptr->start_;
        info->total_++;
        info->total_count_ += ptr->count_;
        info->total_cost_ += tm;
        start = min(start, ptr->start_);
        end = max(end, ptr->start_);
        info->max_cost_ = max(info->max_cost_, tm);
    }

    float time_diff = float(end - start);
    nlohmann::json result = nlohmann::json::array();
    for (auto &iter:ok) {
        iter.second->avg_cost_ = float(iter.second->total_cost_) / float(iter.second->total_);
        iter.second->avg_count_ = float(iter.second->total_count_) / float(iter.second->total_);
        if (time_diff > 0) {
            iter.second->qps_ = float(iter.second->total_) / time_diff;
        }
        nlohmann::json item;
        item["name"] = iter.second->name_;
        item["total"] = iter.second->total_;
        item["qps"] = iter.second->qps_ * 1000;
        item["counter"] = iter.second->total_count_;
        item["avg_counter"] = iter.second->avg_count_;
        item["avg_cost"] = iter.second->avg_cost_;
        item["max_cost"] = iter.second->max_cost_;
        item["status"] = "OK";
        result.push_back(item);
        delete iter.second;
    }

    for (auto &iter:err) {
        iter.second->avg_cost_ = float(iter.second->total_cost_) / float(iter.second->total_);
        iter.second->avg_count_ = float(iter.second->total_count_) / float(iter.second->total_);
        if (time_diff > 0) {
            iter.second->qps_ = float(iter.second->total_) / time_diff;
        }
        nlohmann::json item;
        item["name"] = iter.second->name_;
        item["total"] = iter.second->total_;
        item["qps"] = iter.second->qps_ * 1000;
        item["counter"] = iter.second->total_count_;
        item["avg_counter"] = iter.second->avg_count_;
        item["avg_cost"] = iter.second->avg_cost_;
        item["max_cost"] = iter.second->max_cost_;
        item["status"] = "ERR";
        result.push_back(item);
        delete iter.second;
    }
    stat_info_ = result.dump();
}

std::string &stata::Stata::get_status() {
    return stat_info_;
}

void stata::do_stat_func() {
    while (1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(stata::interval));
        stata::Stata::getStata()->do_stat();
        LOG(INFO) << stata::Stata::getStata()->get_status();
    }
}

stata::Stata *stata::Stata::global_stata_ = new(std::nothrow)stata::Stata;