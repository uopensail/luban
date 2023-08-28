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

  const json &doc = json::parse(buffer);
  const std::vector<json> &transforms = doc["transforms"];

  for (auto &t : transforms) {
    m_funcs.emplace_back(t);
  }
  m_placer = std::make_shared<Placement>(doc);
  m_opr = std::make_shared<Operator>();
}

std::shared_ptr<Rows> Toolkit::process(SharedFeaturesPtr features) {
  auto r = m_placer->rows();
  for (size_t i = 0; i < m_funcs.size(); ++i) {
    m_opr->call(m_funcs[i], *features);
  }
  m_placer->call(*features, r);
  return r;
}

std::shared_ptr<Matrices> Toolkit::process(SharedFeaturesListPtr list) {
  int64_t size = list->size();
  auto m = m_placer->matrices(size);
  for (int64_t i = 0; i < size; ++i) {
    for (size_t j = 0; j < m_funcs.size(); ++j) {
      m_opr->call(m_funcs[j], *(list->operator[](i)));
    }
    m_placer->call(*(list->operator[](i)), m, i);
  }
  return m;
}

}  // namespace luban