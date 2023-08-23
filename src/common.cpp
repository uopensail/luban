#include "common.h"

namespace luban {

FunctionConfigure::FunctionConfigure(const std::string &data) {
  rapidjson::Document doc;
  doc.Parse(data.c_str());

  this->func = doc["func"].GetString();
  this->name = doc["name"].GetString();
  this->flag = doc["flag"].GetInt64();

  for (rapidjson::SizeType j = 0; j < doc["args"].Size(); ++j) {
    this->literals.push_back(parse_parameter(doc["args"][j]));
  }

  for (rapidjson::SizeType j = 0; j < doc["vars"].Size(); ++j) {
    this->variables.push_back(doc["vars"][j].GetString());
  }
}

FunctionConfigure::FunctionConfigure(const rapidjson::Value &doc) {
  func = doc["func"].GetString();
  name = doc["name"].GetString();
  flag = doc["flag"].GetInt64();

  for (rapidjson::SizeType j = 0; j < doc["args"].Size(); ++j) {
    literals.push_back(parse_parameter(doc["args"][j]));
  }

  for (rapidjson::SizeType j = 0; j < doc["vars"].Size(); ++j) {
    variables.push_back(doc["vars"][j].GetString());
  }
}

std::string FunctionConfigure::stringnify() {
  std::string ret(name);
  ret.append(" = ");
  ret.append(func);
  ret.append("(");
  int64_t v = 0, l = 0;
  int64_t len = variables.size() + literals.size();
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
  return ret;
}

SharedParameter parse_parameter(const rapidjson::Value &data) {
  DataType type = static_cast<DataType>(data["type"].GetInt());

  if (type == DataType::kInt64) {
    SharedParameter p = std::make_shared<Parameter>();
    *p = data["value"].GetInt64();
    return p;
  } else if (type == DataType::kFloat32) {
    SharedParameter p = std::make_shared<Parameter>();
    *p = data["value"].GetFloat();
    return p;
  } else if (type == DataType::kString) {
    SharedParameter p = std::make_shared<Parameter>();
    *p = data["value"].GetString();
    return p;
  } else if (type == DataType::kInt64Array) {
    assert(data.IsArray());
    SharedParameter p = std::make_shared<Parameter>();
    std::vector<int64_t> value;
    for (rapidjson::SizeType j = 0; j < data.Size(); ++j) {
      value.push_back(data[j].GetInt64());
    }
    *p = value;
    return p;
  } else if (type == DataType::kFloat32Array) {
    assert(data.IsArray());
    SharedParameter p = std::make_shared<Parameter>();
    std::vector<float> value;
    for (rapidjson::SizeType j = 0; j < data.Size(); ++j) {
      value.push_back(data[j].GetFloat());
    }
    *p = value;
    return p;
  } else if (type == DataType::kStringArray) {
    assert(data.IsArray());
    SharedParameter p = std::make_shared<Parameter>();
    std::vector<std::string> value;
    for (rapidjson::SizeType j = 0; j < data.Size(); ++j) {
      value.push_back(data[j].GetString());
    }
    *p = value;
    return p;
  }
  return nullptr;
}

SharedParameter parse_parameter(const std::string &data) {
  rapidjson::Document doc;
  doc.Parse(data.c_str());
  return parse_parameter(doc);
}

std::string parameter_stringnify(const SharedParameter &param) {
  std::string str("");

  if (param == nullptr) {
    str.append("<nil>");
    return str;
  }
  if (auto p = std::get_if<float>(param.get())) {
    str.append(std::to_string(*p));
  } else if (auto p = std::get_if<std::vector<float>>(param.get())) {
    str.append("[");
    for (size_t i = 0; i < p->size(); ++i) {
      if (i != 0) {
        str.append(",");
      }
      str.append(std::to_string((*p)[i]));
    }
    str.append("]");
  } else if (auto p = std::get_if<std::vector<int64_t>>(param.get())) {
    str.append("[");
    for (size_t i = 0; i < p->size(); ++i) {
      if (i != 0) {
        str.append(",");
      }
      str.append(std::to_string((*p)[i]));
    }
    str.append("]");
  } else if (auto p = std::get_if<int64_t>(param.get())) {
    str.append(std::to_string(*p));
  } else if (auto p = std::get_if<std::vector<std::string>>(param.get())) {
    str.append("[");
    for (size_t i = 0; i < p->size(); ++i) {
      if (i != 0) {
        str.append(",");
      }
      str.append("\"");
      str.append((*p)[i]);
      str.append("\"");
    }
    str.append("]");
  } else if (auto p = std::get_if<std::string>(param.get())) {
    str.append("\"");
    str.append(*p);
    str.append("\"");
  }
  return str;
}

void print_parameter(const SharedParameter &param) {
  std::cout << parameter_stringnify(param) << std::endl;
}

void extract_features(std::string_view data, SharedParameterMap &features) {
  rapidjson::Document doc;
  doc.Parse(data.data(), data.size());

  assert(doc.IsArray());

  for (rapidjson::SizeType i = 0; i < doc.Size(); ++i) {
    int type = doc[i]["type"].GetInt();
    std::string name = doc[i]["name"].GetString();
    SharedParameter p = parse_parameter(doc[i]);
    if (p != nullptr) {
      features[name] = p;
    }
  }
}

}  // namespace luban