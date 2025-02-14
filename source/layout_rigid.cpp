#include <stdexcept>
#include <unordered_set>

#include "display/layout_rigid.hpp"

namespace display
{

LayoutRigid::LayoutRigid(layout_t layout)
    : Layout(apos_t(0, 0))
    , m_grid(static_cast<sz_t>(layout[0].size()),
             static_cast<sz_t>(layout.size()))
    , m_recs(count_and_pad(layout))
{
  static const auto& insert =
      [](sz_t& count, std::uint8_t cnt, sz_t& pos, std::uint8_t total)
  {
    if (count != 0 && (pos != total || count != cnt)) {
      throw std::runtime_error("Invalid layout [Shape]");
    }

    if (count != 0) {
      return;
    }

    pos = total;
    count = cnt;
  };

  for (std::size_t i = 0U; i < m_grid.height; i++) {
    uint8_t total = 0;
    uint8_t cnt = 1;

    for (std::size_t j = 0U; j < m_grid.width; j++, cnt++) {
      const auto crnt = layout[i][j];

      if (crnt == layout[i][j + 1]) {
        continue;
      }

      insert(m_recs[crnt].dim.width, cnt, m_recs[crnt].start.width, total);
      total += cnt, cnt = 0;
    }
  }

  for (std::size_t j = 0U; j < m_grid.width; j++) {
    uint8_t total = 0;
    uint8_t cnt = 1;

    for (std::size_t i = 0U; i < m_grid.height; i++, cnt++) {
      const auto crnt = layout[i][j];

      if (crnt == layout[i + 1][j]) {
        continue;
      }

      insert(m_recs[crnt].dim.height, cnt, m_recs[crnt].start.height, total);
      total += cnt, cnt = 0;
    }
  }
}

std::size_t LayoutRigid::count_and_pad(layout_t& layout) const
{
  std::unordered_set<std::uint8_t> ust;

  for (std::size_t i = 0U; i < m_grid.height; i++) {
    for (std::size_t j = 0U; j < m_grid.width; j++) {
      ust.insert(layout[i][j]);
    }
    layout[i].emplace_back(0xFF);
  }
  layout.emplace_back(m_grid.width, 0xFF);

  for (std::size_t i = 0U; i < m_grid.height; i++) {
    for (std::size_t j = 0U; j < m_grid.width; j++) {
      if (layout[i][j] >= ust.size()) {
        throw std::runtime_error("Invalid layout [Number]");
      }
    }
  }

  return ust.size();
}

auto LayoutRigid::calc_width(dim_t share) const
{
  return static_cast<sz_t>(dim().width / m_grid.width * share.width);
}

auto LayoutRigid::calc_height(dim_t share) const
{
  return static_cast<sz_t>(dim().height / m_grid.height * share.height);
}

void LayoutRigid::resize(apos_t apos, dim_t dim)
{
  Layout::resize(apos, dim);

  for (auto& [screen, start, rdim] : m_recs) {
    const dim_t size = {calc_width(rdim), calc_height(rdim)};
    const pos_t rel = {calc_width(start), calc_height(start)};
    const apos_t abs = this->apos() + rel;
    screen.resize(abs, size);
  }
}

void LayoutRigid::render() const
{
  for (const auto& [screen, _, __] : m_recs) {
    screen.render();
  }
}

}  // namespace display
