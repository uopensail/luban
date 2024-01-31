#include "operator.h"

namespace luban {
Wrapper::Wrapper(const FunctionConfigure &conf, Features &features)
    : m_index(0), m_lit(0), m_var(0), m_feats(features), m_conf(conf) {}

Wrapper::Wrapper(const Wrapper &w)
    : m_index(w.m_index), m_lit(w.m_lit), m_var(w.m_var), m_feats(w.m_feats),
      m_conf(w.m_conf) {}

Wrapper::Wrapper(const Wrapper &&w)
    : m_index(w.m_index), m_lit(w.m_lit), m_var(w.m_var), m_feats(w.m_feats),
      m_conf(w.m_conf) {}

bool contain_nullptr() { return false; }

Operator::Operator() {
  m_funcs["now"] = get_func_call(now);
  m_funcs["year"] = get_func_call(year);
  m_funcs["month"] = get_func_call(month);
  m_funcs["day"] = get_func_call(day);
  m_funcs["hour"] = get_func_call(hour);
  m_funcs["minute"] = get_func_call(minute);
  m_funcs["second"] = get_func_call(second);
  m_funcs["date"] = get_func_call(date);
  m_funcs["addf"] = get_func_call(_add<float>);
  m_funcs["addi"] = get_func_call(_add<int64_t>);
  m_funcs["add"] = get_func_call(_add<float>);
  m_funcs["+"] = get_func_call(_add<float>);
  m_funcs["sub"] = get_func_call(_sub<float>);
  m_funcs["-"] = get_func_call(_sub<float>);
  m_funcs["subf"] = get_func_call(_sub<float>);
  m_funcs["subi"] = get_func_call(_sub<int64_t>);
  m_funcs["mulf"] = get_func_call(_mul<float>);
  m_funcs["muli"] = get_func_call(_mul<int64_t>);
  m_funcs["mul"] = get_func_call(_mul<float>);
  m_funcs["*"] = get_func_call(_mul<float>);
  m_funcs["divf"] = get_func_call(_div<float>);
  m_funcs["div"] = get_func_call(_div<float>);
  m_funcs["/"] = get_func_call(_div<float>);
  m_funcs["divi"] = get_func_call(_div<int64_t>);
  m_funcs["mod"] = get_func_call(_mod);
  m_funcs["%"] = get_func_call(_mod);
  m_funcs["pow"] = get_func_call(_pow);
  m_funcs["round"] = get_func_call(_round);
  m_funcs["floor"] = get_func_call(_floor);
  m_funcs["ceil"] = get_func_call(_ceil);
  m_funcs["floorf"] = get_func_call(_floorf);
  m_funcs["ceilf"] = get_func_call(_ceilf);
  m_funcs["log"] = get_func_call(_log);
  m_funcs["exp"] = get_func_call(_exp);
  m_funcs["log10"] = get_func_call(_log10);
  m_funcs["log2"] = get_func_call(_log2);
  m_funcs["sqrt"] = get_func_call(_sqrt);
  m_funcs["absf"] = get_func_call(_abs<float>);
  m_funcs["abs"] = get_func_call(_abs<float>);
  m_funcs["absi"] = get_func_call(_abs<int64_t>);
  m_funcs["sin"] = get_func_call(_sin);
  m_funcs["sinh"] = get_func_call(_sinh);
  m_funcs["asin"] = get_func_call(_asin);
  m_funcs["asinh"] = get_func_call(_asinh);
  m_funcs["cos"] = get_func_call(_cos);
  m_funcs["cosh"] = get_func_call(_cosh);
  m_funcs["acos"] = get_func_call(_acos);
  m_funcs["acosh"] = get_func_call(_acosh);
  m_funcs["tan"] = get_func_call(_tan);
  m_funcs["tanh"] = get_func_call(_tanh);
  m_funcs["atan"] = get_func_call(_atan);
  m_funcs["atanh"] = get_func_call(_atanh);
  m_funcs["sigmoid"] = get_func_call(_sigmoid);
  m_funcs["minf"] = get_func_call(min<float>);
  m_funcs["mini"] = get_func_call(min<int64_t>);
  m_funcs["min"] = get_func_call(min<float>);
  m_funcs["maxf"] = get_func_call(max<float>);
  m_funcs["maxi"] = get_func_call(max<int64_t>);
  m_funcs["max"] = get_func_call(max<float>);
  m_funcs["average"] = get_func_call(average<float>);
  m_funcs["averagef"] = get_func_call(average<float>);
  m_funcs["averagei"] = get_func_call(average<int64_t>);
  m_funcs["stddev"] = get_func_call(stddev<float>);
  m_funcs["stddevf"] = get_func_call(stddev<float>);
  m_funcs["stddevi"] = get_func_call(stddev<int64_t>);
  m_funcs["variance"] = get_func_call(variance<float>);
  m_funcs["variancef"] = get_func_call(variance<float>);
  m_funcs["variancei"] = get_func_call(variance<int64_t>);
  m_funcs["norm"] = get_func_call(norm<float>);
  m_funcs["normf"] = get_func_call(norm<float>);
  m_funcs["normi"] = get_func_call(norm<int64_t>);
  m_funcs["from_unixtime"] = get_func_call(from_unixtime);
  m_funcs["unix_timestamp"] = get_func_call(unix_timestamp);
  m_funcs["date_add"] = get_func_call(date_add);
  m_funcs["date_sub"] = get_func_call(date_sub);
  m_funcs["date_diff"] = get_func_call(date_diff);
  m_funcs["reverse"] = get_func_call(reverse);
  m_funcs["upper"] = get_func_call(upper);
  m_funcs["lower"] = get_func_call(lower);
  m_funcs["substr"] = get_func_call(substr);
  m_funcs["concat"] = get_func_call(concat);
  m_funcs["min_maxf"] = get_func_call(min_max<float>);
  m_funcs["min_maxi"] = get_func_call(min_max<int64_t>);
  m_funcs["min_max"] = get_func_call(min_max<float>);
  m_funcs["z_score"] = get_func_call(z_score);
  m_funcs["binarize"] = get_func_call(binarize<float>);
  m_funcs["binarizef"] = get_func_call(binarize<float>);
  m_funcs["binarizei"] = get_func_call(binarize<int64_t>);
  m_funcs["bucketize"] = get_func_call(bucketize<float>);
  m_funcs["bucketizef"] = get_func_call(bucketize<float>);
  m_funcs["bucketizei"] = get_func_call(bucketize<int64_t>);
  m_funcs["box_cox"] = get_func_call(box_cox);
  m_funcs["normalize"] = get_func_call(normalize<float>);
  m_funcs["normalizef"] = get_func_call(normalize<float>);
  m_funcs["normalizei"] = get_func_call(normalize<int64_t>);
  m_funcs["topkf"] = get_func_call(topk<float>);
  m_funcs["topks"] = get_func_call(topk<std::string>);
  m_funcs["topki"] = get_func_call(topk<int64_t>);
  m_funcs["topk"] = get_func_call(topk<std::string>);
  m_funcs["cross"] = get_func_call(cross);
  m_funcs["concat_ws"] = get_func_call(concat_ws);
}

void Operator::call(const FunctionConfigure &conf, Features &features) {
  auto iter = m_funcs.find(conf.func);
  if (iter == m_funcs.end()) {
    return;
  }
  const SharedParameter &p = iter->second(conf, features);
  if (p != nullptr) {
    features.set(conf.name, p);
  }
}

} // namespace luban