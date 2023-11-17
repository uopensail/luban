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
  infile.close();
  m_opr = std::make_shared<Operator>();

  const json &doc = json::parse(buffer);
  const json &user = doc["user"];
  const json &item = doc["item"];

  const std::vector<json> &user_transforms = user["transforms"];
  const std::vector<json> &item_transforms = item["transforms"];
  for (auto &t : user_transforms) {
    m_user_funcs.emplace_back(t);
  }
  for (auto &t : item_transforms) {
    m_item_funcs.emplace_back(t);
  }

  m_user_placer = std::make_shared<Placement>(user);
  m_item_placer = std::make_shared<Placement>(item);

  m_groups.resize(m_user_placer->m_groups.size() +
                  m_item_placer->m_groups.size());

  for (size_t i = 0; i < m_user_placer->m_groups.size(); i++) {
    m_groups[m_user_placer->m_groups[i].id] = m_user_placer->m_groups[i];
  }

  for (size_t i = 0; i < m_item_placer->m_groups.size(); i++) {
    m_groups[m_item_placer->m_groups[i].id] = m_item_placer->m_groups[i];
  }
}

std::shared_ptr<Rows> Toolkit::process_item(SharedFeaturesPtr features) {
  auto r = m_item_placer->rows();
  for (size_t i = 0; i < m_item_funcs.size(); i++) {
    m_opr->call(m_item_funcs[i], *features);
  }
  m_item_placer->call(*features, r);
  return r;
}
std::shared_ptr<Rows> Toolkit::process_user(SharedFeaturesPtr features) {
  auto r = m_user_placer->rows();
  for (size_t i = 0; i < m_user_funcs.size(); ++i) {
    m_opr->call(m_user_funcs[i], *features);
  }
  m_user_placer->call(*features, r);
  return r;
}

std::shared_ptr<Rows> Toolkit::process(SharedFeaturesPtr features) {
  auto r = std::make_shared<Rows>();
  r->m_rows.resize(m_groups.size());
  auto items = process_item(features);

  for (size_t i = 0; i < m_item_placer->m_groups.size(); i++) {
    r->m_rows[m_item_placer->m_groups[i].id] =
        items->m_rows[m_item_placer->m_groups[i].index];
  }

  auto user = process_user(features);

  for (size_t i = 0; i < m_user_placer->m_groups.size(); i++) {
    r->m_rows[m_user_placer->m_groups[i].id] =
        user->m_rows[m_user_placer->m_groups[i].index];
  }
  user->m_rows.clear();
  items->m_rows.clear();
  return r;
}

} // namespace luban
