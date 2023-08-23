#include "placement.h"

namespace luban {

Row::Row(DataType type, int64_t cols) : m_type(type), m_cols(cols) {
  if (type == DataType::kInt64) {
    m_data = (char *)malloc(sizeof(int64_t) * m_cols);
    m_size = sizeof(int64_t);
  } else if (type == DataType::kFloat32) {
    m_data = (char *)malloc(sizeof(float) * m_cols);
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
    m_data = (char *)malloc(sizeof(int64_t) * m_rows * m_cols);
    m_size = sizeof(int64_t);
  } else if (type == DataType::kFloat32) {
    m_data = (char *)malloc(sizeof(float) * m_rows * m_cols);
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

Placement::Placement(rapidjson::Value &doc) {
  m_groups.resize(doc["groups"].Size());
  for (rapidjson::SizeType i = 0; i < doc["groups"].Size(); ++i) {
    int id = doc["groups"][i]["id"].GetInt();
    int64_t width = doc["groups"][i]["width"].GetInt64();
    DataType type = static_cast<DataType>(doc["groups"][i]["type"].GetInt());
    assert(type == DataType::kInt64 || type == DataType::kFloat32);
    m_groups[id] = GroupConfigure{id, width, type};
  }

  for (rapidjson::SizeType i = 0; i < doc["features"].Size(); ++i) {
    int group = doc["features"][i]["group"].GetInt();
    std::string name = doc["features"][i]["name"].GetString();
    int dim = doc["features"][i]["dim"].GetInt();
    bool hash = doc["features"][i]["hash"].GetBool();
    int64_t offset = doc["features"][i]["offset"].GetInt64();
    DataType type = static_cast<DataType>(doc["features"][i]["type"].GetInt());
    DataType gtype = m_groups[group].type;
    Padding padding;
    if (type == DataType::kInt64) {
      padding = doc["features"][i]["padding"].GetInt64();
    } else if (type == DataType::kFloat32) {
      padding = doc["features"][i]["padding"].GetFloat();
    }

    m_features[name] =
        FeatureConfigure{hash, type, group, dim, offset, padding};
  }
}

Placement::Placement(std::string_view config) {
  rapidjson::Document doc;
  doc.Parse(config.data(), config.size());

  m_groups.resize(doc["groups"].Size());
  for (rapidjson::SizeType i = 0; i < doc["groups"].Size(); ++i) {
    int id = doc["groups"][i]["id"].GetInt();
    int64_t width = doc["groups"][i]["width"].GetInt64();
    DataType type = static_cast<DataType>(doc["groups"][i]["type"].GetInt());
    assert(type == DataType::kInt64 || type == DataType::kFloat32);
    m_groups[id] = GroupConfigure{id, width, type};
  }

  for (rapidjson::SizeType i = 0; i < doc["features"].Size(); ++i) {
    int group = doc["features"][i]["group"].GetInt();
    std::string name = doc["features"][i]["name"].GetString();
    int dim = doc["features"][i]["dim"].GetInt();
    bool hash = doc["features"][i]["hash"].GetBool();
    int64_t offset = doc["features"][i]["offset"].GetInt64();
    DataType type = static_cast<DataType>(doc["features"][i]["type"].GetInt());
    DataType gtype = m_groups[group].type;
    Padding padding;
    if (type == DataType::kInt64) {
      padding = doc["features"][i]["padding"].GetInt64();
    } else if (type == DataType::kFloat32) {
      padding = doc["features"][i]["padding"].GetFloat();
    }

    m_features[name] =
        FeatureConfigure{hash, type, group, dim, offset, padding};
  }
}

// duto RVO, return vector is ok
Matrices Placement::matrices(int64_t batch_size) {
  Matrices m;
  for (int64_t i = 0; i < m_groups.size(); i++) {
    m.push_back(std::make_shared<Matrix>(m_groups[i].type, batch_size,
                                         m_groups[i].width));
  }
  return m;
}

Rows Placement::rows() {
  Rows r;
  for (int64_t i = 0; i < m_groups.size(); i++) {
    r.push_back(std::make_shared<Row>(m_groups[i].type, m_groups[i].width));
  }
  return r;
}

void Placement::call(Features &features, Matrices &m, int64_t row) {
  for (auto &kv : m_features) {
    m[kv.second.group]->put(row, features[kv.first], kv.second);
  }
}

void Placement::call(Features &features, Rows &r) {
  for (auto &kv : m_features) {
    r[kv.second.group]->put(features[kv.first], kv.second);
  }
}

}  // namespace luban