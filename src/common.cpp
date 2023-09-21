#include "common.h"

namespace luban {

FunctionConfigure::FunctionConfigure(const std::string &data) {
  const json &doc = json::parse(data);
  parse(doc);
}

FunctionConfigure::FunctionConfigure(const json &doc) { parse(doc); }

void FunctionConfigure::parse(const json &doc) {
  func = doc["func"].get<std::string>();
  name = doc["name"].get<std::string>();
  flag = doc["flag"].get<int64_t>();
  std::vector<json> args = doc["args"];
  variables = doc["vars"].get<std::vector<std::string>>();
  for (auto &arg : args) {
    literals.push_back(parse_parameter_from_json(arg));
  }

#ifdef __GCC__
  // gcc, default cdecl, function params push to stack from right to left
  std::reverse(literals.begin(), literals.end());
  std::reverse(variables.begin(), variables.end());
  size_t len = variables.size() + literals.size();
  int64_t reversed_flag = 0;

  for (size_t i = 0; i < len; i++) {
    if ((1 << i) & flag) {
      reversed_flag += (1 << (len - 1 - i));
    }
  }
  flag = reversed_flag;

#endif
}

std::string FunctionConfigure::stringnify() {
  std::string ret(name);
  ret.append(" = ");
  ret.append(func);
  ret.append("(");

  int64_t len = variables.size() + literals.size();
#ifdef __GCC__
  int64_t v = variables.size() - 1, l = literals.size() - 1;
  for (int64_t i = len - 1; i >= 0; i--) {
    if (i != len - 1) {
      ret.append(",");
    }
    if ((1 << i) & flag) {
      ret.append("`" + variables[v--] + "`");
    } else {
      ret.append(parameter_stringnify(literals[l--]));
    }
  }
  ret.append(")");
#else
  int64_t v = 0, l = 0;
  for (int64_t i = 0; i < len; i++) {
    if (i) {
      ret.append(",");
    }
    if ((1 << i) & flag) {
      ret.append("`" + variables[v++] + "`");
    } else {
      ret.append(parameter_stringnify(literals[l++]));
    }
  }
  ret.append(")");
#endif
  return ret;
}

void print_parameter(const SharedParameter &param) {
  std::cout << parameter_stringnify(param) << std::endl;
}

std::string parameter_stringnify(const SharedParameter &param) {
  if (param == nullptr) {
    return "null";
  }
  json j;
  auto foo = [&j](auto &&arg) { j = arg; };
  std::visit(foo, *param);
  return j.dump();
}

SharedParameter parse_parameter_from_string(const std::string &data) {
  const json &doc = json::parse(data);
  return parse_parameter_from_json(doc);
}

SharedParameter parse_parameter_from_json(const json &data) {
  DataType type = static_cast<DataType>(data["type"].get<int>());

  if (type == DataType::kInt64) {
    SharedParameter p = std::make_shared<Parameter>();
    *p = data["value"].get<int64_t>();
    return p;
  } else if (type == DataType::kFloat32) {
    SharedParameter p = std::make_shared<Parameter>();
    *p = data["value"].get<float>();
    return p;
  } else if (type == DataType::kString) {
    SharedParameter p = std::make_shared<Parameter>();
    *p = data["value"].get<std::string>();
    return p;
  } else if (type == DataType::kInt64Array) {
    SharedParameter p = std::make_shared<Parameter>();
    *p = data["value"].get<std::vector<int64_t>>();
    return p;
  } else if (type == DataType::kFloat32Array) {
    SharedParameter p = std::make_shared<Parameter>();
    *p = data["value"].get<std::vector<float>>();
    return p;
  } else if (type == DataType::kStringArray) {
    SharedParameter p = std::make_shared<Parameter>();
    *p = data["value"].get<std::vector<std::string>>();
    return p;
  }
  return nullptr;
}

void extract_features(std::string_view data, SharedParameterMap &features) {
  const std::map<std::string, json> &doc = json::parse(data);
  for (const auto &kv : doc) {
    SharedParameter p = parse_parameter_from_json(kv.second);
    if (p != nullptr) {
      features[kv.first] = p;
    }
  }
}

} // namespace luban