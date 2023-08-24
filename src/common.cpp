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
    assert(data["value"].IsArray());
    SharedParameter p = std::make_shared<Parameter>();
    std::vector<int64_t> value;
    for (rapidjson::SizeType j = 0; j < data["value"].Size(); ++j) {
      value.push_back(data["value"][j].GetInt64());
    }
    *p = value;
    return p;
  } else if (type == DataType::kFloat32Array) {
    assert(data["value"].IsArray());
    SharedParameter p = std::make_shared<Parameter>();
    std::vector<float> value;
    for (rapidjson::SizeType j = 0; j < data["value"].Size(); ++j) {
      value.push_back(data["value"][j].GetFloat());
    }
    *p = value;
    return p;
  } else if (type == DataType::kStringArray) {
    assert(data["value"].IsArray());
    SharedParameter p = std::make_shared<Parameter>();
    std::vector<std::string> value;
    for (rapidjson::SizeType j = 0; j < data["value"].Size(); ++j) {
      value.push_back(data["value"][j].GetString());
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
  auto value = parameter_jsonify(param);
  if (value == nullptr) {
    return "";
  }
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  value->Accept(writer);
  return buffer.GetString();
}

std::shared_ptr<rapidjson::Value> parameter_jsonify(
    const SharedParameter &param) {
  if (param == nullptr) {
    return nullptr;
  }

  if (auto p = std::get_if<float>(param.get())) {
    auto value = std::make_shared<rapidjson::Value>();
    value->SetFloat(*p);
    return value;
  } else if (auto p = std::get_if<std::vector<float>>(param.get())) {
    auto array = std::make_shared<rapidjson::Value>(rapidjson::kArrayType);
    for (size_t i = 0; i < p->size(); ++i) {
      array->PushBack((*p)[i], rapidjson::Document().GetAllocator());
    }
    return array;
  } else if (auto p = std::get_if<std::vector<int64_t>>(param.get())) {
    auto array = std::make_shared<rapidjson::Value>(rapidjson::kArrayType);
    for (size_t i = 0; i < p->size(); ++i) {
      array->PushBack((*p)[i], rapidjson::Document().GetAllocator());
    }
    return array;
  } else if (auto p = std::get_if<int64_t>(param.get())) {
    auto value = std::make_shared<rapidjson::Value>();
    value->SetInt64(*p);
    return value;
  } else if (auto p = std::get_if<std::vector<std::string>>(param.get())) {
    auto array = std::make_shared<rapidjson::Value>(rapidjson::kArrayType);
    for (size_t i = 0; i < p->size(); ++i) {
      std::string &s = (*p)[i];
      array->PushBack(
          rapidjson::Value(s.c_str(),
                           static_cast<rapidjson::SizeType>(s.size())),
          rapidjson::Document().GetAllocator());
    }
    return array;
  } else if (auto p = std::get_if<std::string>(param.get())) {
    auto value = std::make_shared<rapidjson::Value>();
    std::string &s = *p;
    value->SetString(s.c_str(), static_cast<rapidjson::SizeType>(s.size()),
                     rapidjson::Document().GetAllocator());
    return value;
  }
  return nullptr;
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