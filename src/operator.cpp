#include "operator.h"

namespace luban {
Wrapper::Wrapper(const FunctionConfigure &conf, Features &features)
    : m_index(0), m_lit(0), m_var(0), m_feats(features), m_conf(conf) {}

Wrapper::Wrapper(const Wrapper &w)
    : m_index(w.m_index),
      m_lit(w.m_lit),
      m_var(w.m_var),
      m_feats(w.m_feats),
      m_conf(w.m_conf) {}

Wrapper::Wrapper(const Wrapper &&w)
    : m_index(w.m_index),
      m_lit(w.m_lit),
      m_var(w.m_var),
      m_feats(w.m_feats),
      m_conf(w.m_conf) {}

bool contain_nullptr() { return false; }

Operator::Operator() {
#define add_caller(func) \
  { m_funcs[#func] = get_func_call(func); }

  add_caller(now);
  add_caller(year);
  add_caller(month);
  add_caller(day);
  add_caller(hour);
  add_caller(minute);
  add_caller(second);
  add_caller(date);
  add_caller(_add<int64_t>);
  add_caller(_add<float>);
  add_caller(_sub<int64_t>);
  add_caller(_sub<float>);
  add_caller(_mul<int64_t>);
  add_caller(_mul<float>);
  add_caller(_div<int64_t>);
  add_caller(_div<float>);
  add_caller(_mod);
  add_caller(_pow);
  add_caller(_round);
  add_caller(_floor);
  add_caller(_ceil);
  add_caller(_log);
  add_caller(_exp);
  add_caller(_log10);
  add_caller(_log2);
  add_caller(_sqrt);
  add_caller(_abs);
  add_caller(_sin);
  add_caller(_sinh);
  add_caller(_asin);
  add_caller(_asinh);
  add_caller(_cos);
  add_caller(_cosh);
  add_caller(_acos);
  add_caller(_acosh);
  add_caller(_tan);
  add_caller(_tanh);
  add_caller(_atan);
  add_caller(_atanh);
  add_caller(_sigmoid);
  add_caller(min);
  add_caller(max);
  add_caller(stddev);
  add_caller(variance);
  add_caller(norm);
  add_caller(from_unixtime);
  add_caller(unix_timestamp);
  add_caller(date_add);
  add_caller(date_sub);
  add_caller(date_diff);
  add_caller(reverse);
  add_caller(upper);
  add_caller(lower);
  add_caller(substr);
  add_caller(concat);
  add_caller(min_max);
  add_caller(z_score);
  add_caller(binarize);
  add_caller(bucketize);
  add_caller(box_cox);
  add_caller(normalize);
  add_caller(topk<std::string>);
  add_caller(topk<float>);
  add_caller(topk<int64_t>);
  add_caller(cross);
}

void Operator::call(const FunctionConfigure &conf, Features &features) {
  auto iter = m_funcs.find(conf.func);
  if (iter == m_funcs.end()) {
    return;
  }
  const SharedParameter &p = iter->second(conf, features);
  if (p != nullptr) {
    features.insert(conf.name, p);
  }
}

}  // namespace luban