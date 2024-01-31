#include "placement.h"

namespace luban {

Row::Row(DataType type, int64_t cols) : m_type(type), m_cols(cols) {
  if (type == DataType::kInt64) {
    m_size = sizeof(int64_t);
  } else if (type == DataType::kFloat32) {
    m_size = sizeof(float);
  } else {
    throw std::runtime_error("type error: " + std::to_string(type));
  }
  m_data = (char *)calloc(m_cols, m_size);
}

Row::~Row() {
  if (m_data != nullptr) {
    free(m_data);
    m_data = nullptr;
  }
}

void Row::put(SharedParameter value, const FeatureConfigure &conf) {
  if (m_type == DataType::kInt64) {
    int64_t *ptr = (int64_t *)(m_data + conf.offset * m_size);
    auto array = placement<int64_t>(value, conf.hash, conf.padding, conf.dim);
    for (size_t i = 0; i < array.size(); ++i) {
      ptr[i] = array[i];
    }
  } else if (m_type == DataType::kFloat32) {
    float *ptr = (float *)(m_data + conf.offset * m_size);
    auto array = placement<float>(value, conf.hash, conf.padding, conf.dim);
    for (size_t i = 0; i < array.size(); ++i) {
      ptr[i] = array[i];
    }
  }
}

int64_t Rows::size() { return m_rows.size(); }

std::shared_ptr<Row> Rows::operator[](int index) { return m_rows[index]; }

std::shared_ptr<json> Rows::to_json() {
  auto js = std::make_shared<json>();
  for (int i = 0;i < m_rows.size();i++) {
    auto row = m_rows[i];
    if (row->m_type == DataType::kInt64) {
      int64_t *ptr = (int64_t *)row->m_data;
      std::vector<int64_t> vv(ptr, ptr+row->m_cols);
      js->push_back(vv);
    } else if (row->m_type == DataType::kFloat32) {
      float *ptr = (float *)row->m_data;
      std::vector<float> vv(ptr, ptr+row->m_cols);
      js->push_back(vv);
    }
  }
  return js;
}
std::string Rows::to_json_str() {
   auto js = this->to_json();
   return js->dump();
}
void Placement::parse(const json &doc) {
  const std::vector<json> &groups = doc["groups"];
  m_groups.resize(groups.size());
  for (auto &group : groups) {
    int index = group["index"];
    int id = group["id"];
    int64_t width = group["width"];
    DataType type = group["type"];

    assert(type == DataType::kInt64 || type == DataType::kFloat32);
    int64_t stride =
        (type == DataType::kInt64 ? sizeof(int64_t) : sizeof(float));
    m_groups[index] = GroupConfigure{id, index, width, stride, type};
  }

  const std::vector<json> &features = doc["features"];
  for (auto &feature : features) {
    int index = feature["index"];
    std::string name = feature["name"];
    int dim = feature["dim"];
    bool hash = feature["hash"];
    int64_t offset = feature["offset"];
    DataType type = feature["type"];
    DataType gtype = m_groups[index].type;
    Padding padding;
    if (type == DataType::kInt64) {
      padding = feature["padding"].get<int64_t>();
    } else if (type == DataType::kFloat32) {
      padding = feature["padding"].get<float>();
    }

    m_features[name] = FeatureConfigure{
        hash, type, m_groups[index].id, index, dim, offset, padding};
  }
}

Placement::Placement(const json &doc) { parse(doc); }

Placement::Placement(std::string_view config) {
  const json &doc = json::parse(config);
  parse(doc);
}

std::shared_ptr<Rows> Placement::rows() {
  auto r = std::make_shared<Rows>();
  r->m_rows.resize(m_groups.size());
  for (int64_t i = 0; i < m_groups.size(); i++) {
    r->m_rows[m_groups[i].index] =
        std::make_shared<Row>(m_groups[i].type, m_groups[i].width);
  }
  return r;
}

void Placement::call(Features &features, std::shared_ptr<Rows> &r) {
  for (auto &kv : m_features) {
    (*r)[kv.second.index]->put(features[kv.first], kv.second);
  }
}

} // namespace luban