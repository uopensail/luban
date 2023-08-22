#include "toolkit.h"

namespace luban {

Toolkit::Toolkit(const std::string &config_file) {
  std::ifstream infile(config_file);
  if (!infile.is_open()) {
    std::cerr << "failed to open file: " << config_file << std::endl;
    return;
  }

  infile.seekg(0, std::ios::end);
  std::string buffer(infile.tellg(), ' ');
  infile.seekg(0);
  infile.read(&buffer[0], buffer.size());

  rapidjson::Document doc;
  doc.Parse(buffer.c_str());

  for (rapidjson::SizeType i = 0; i < doc["transforms"].Size(); ++i) {
    auto &v = doc["transforms"][i];
    m_funcs.emplace_back(v);
  }
}

void Toolkit::process(Features &features, Rows &r) {
  for (size_t i = 0; i < m_funcs.size(); ++i) {
    m_opr->call(m_funcs[i], features);
  }
  m_placer->call(features, r);
}

void Toolkit::process(Features &features, Matrices &m, int64_t row) {
  for (size_t i = 0; i < m_funcs.size(); ++i) {
    m_opr->call(m_funcs[i], features);
  }
  m_placer->call(features, m, row);
}

}  // namespace luban