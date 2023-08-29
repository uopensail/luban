#include "placement.h"

namespace luban {

Row::Row(DataType type, int64_t cols) : m_type(type), m_cols(cols) {
  if (type == DataType::kInt64) {
    m_data = (char *)calloc(m_cols, sizeof(int64_t));
    m_size = sizeof(int64_t);
  } else if (type == DataType::kFloat32) {
    m_data = (char *)calloc(m_cols, sizeof(float));
    m_size = sizeof(float);
  } else {
    throw std::runtime_error("type error: " + std::to_string(type));
  }
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

Matrix::Matrix(DataType type, int64_t rows, int64_t cols)
    : m_type(type), m_rows(rows), m_cols(cols) {
  if (type == DataType::kInt64) {
    m_data = (char *)calloc(m_rows * m_cols, sizeof(int64_t));
    m_size = sizeof(int64_t);
  } else if (type == DataType::kFloat32) {
    m_data = (char *)calloc(m_rows * m_cols, sizeof(float));
    m_size = sizeof(float);
  } else {
    throw std::runtime_error("type error: " + std::to_string(type));
  }
}

Matrix::~Matrix() {
  if (m_data != nullptr) {
    free(m_data);
    m_data = nullptr;
  }
}

void Matrix::put(int64_t row, SharedParameter value,
                 const FeatureConfigure &conf) {
  if (m_type == DataType::kInt64) {
    int64_t *ptr = (int64_t *)(m_data + (row * m_cols + conf.offset) * m_size);
    auto array = placement<int64_t>(value, conf.hash, conf.padding, conf.dim);
    for (size_t i = 0; i < array.size(); ++i) {
      ptr[i] = array[i];
    }
  } else if (m_type == DataType::kFloat32) {
    float *ptr = (float *)(m_data + (row * m_cols + conf.offset) * m_size);
    auto array = placement<float>(value, conf.hash, conf.padding, conf.dim);
    for (size_t i = 0; i < array.size(); ++i) {
      ptr[i] = array[i];
    }
  }
}

int64_t Rows::size() { return m_rows.size(); }

int64_t Matrices::size() { return m_matrices.size(); }

std::shared_ptr<Matrix> Matrices::operator[](int index) {
  return m_matrices[index];
}

std::shared_ptr<Row> Rows::operator[](int index) { return m_rows[index]; }

void Placement::parse(const json &doc) {
  const std::vector<json> &groups = doc["groups"];
  m_groups.resize(groups.size());
  for (auto &group : groups) {
    int id = group["id"].get<int>();
    int64_t width = group["width"].get<int64_t>();
    DataType type = static_cast<DataType>(group["type"].get<int>());
    assert(type == DataType::kInt64 || type == DataType::kFloat32);
    m_groups[id] = GroupConfigure{id, width, type};
  }

  const std::vector<json> &features = doc["features"];
  for (auto &feature : features) {
    int group = feature["group"].get<int>();
    std::string name = feature["name"].get<std::string>();
    int dim = feature["dim"].get<int>();
    bool hash = feature["hash"].get<bool>();
    int64_t offset = feature["offset"].get<int64_t>();
    DataType type = static_cast<DataType>(feature["type"].get<int>());
    DataType gtype = m_groups[group].type;
    Padding padding;
    if (type == DataType::kInt64) {
      padding = feature["padding"].get<int64_t>();
    } else if (type == DataType::kFloat32) {
      padding = feature["padding"].get<float>();
    }

    m_features[name] =
        FeatureConfigure{hash, type, group, dim, offset, padding};
  }
}

Placement::Placement(const json &doc) { parse(doc); }

Placement::Placement(std::string_view config) {
  const json &doc = json::parse(config);
  parse(doc);
}

std::shared_ptr<Matrices> Placement::matrices(int64_t batch_size) {
  auto m = std::make_shared<Matrices>();
  for (int64_t i = 0; i < m_groups.size(); i++) {
    m->m_matrices.push_back(std::make_shared<Matrix>(
        m_groups[i].type, batch_size, m_groups[i].width));
  }
  return m;
}

std::shared_ptr<Rows> Placement::rows() {
  auto r = std::make_shared<Rows>();
  for (int64_t i = 0; i < m_groups.size(); i++) {
    r->m_rows.push_back(
        std::make_shared<Row>(m_groups[i].type, m_groups[i].width));
  }
  return r;
}

void Placement::call(Features &features, std::shared_ptr<Matrices> &m,
                     int64_t row) {
  for (auto &kv : m_features) {
    (*m)[kv.second.group]->put(row, features[kv.first], kv.second);
  }
}

void Placement::call(Features &features, std::shared_ptr<Rows> &r) {
  for (auto &kv : m_features) {
    (*r)[kv.second.group]->put(features[kv.first], kv.second);
  }
}

}  // namespace luban