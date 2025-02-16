#pragma once

#include <stdexcept>
#include <unordered_set>
#include <vector>

#include "display/layout.hpp"
#include "display/types.hpp"

namespace display
{

template<typename T = Element>
class LayoutRigid : public LayoutMulti<T>
{
public:
  using layout_t = std::vector<std::vector<std::uint8_t>>;

  LayoutRigid(aplace_t aplc, layout_t layout);  // NOLINT

private:
  std::size_t count_and_pad(layout_t& layout) const;

  aplace_t place(std::size_t idx) const override
  {
    const auto calc = [&]<typename R>(dim_t share) -> R
    {
      return {static_cast<sz_t>(this->awth() / m_grid.width * share.width),
              static_cast<sz_t>(this->ahgt() / m_grid.height * share.height)};
    };

    const auto start = calc.template operator()<pos_t>(m_recs[idx].start);
    const auto dim = calc.template operator()<dim_t>(m_recs[idx].dim);
    return {this->apos() + start, dim};
  }

  dim_t m_grid;

  struct record_t
  {
    dim_t start = {0xFFFF, 0xFFFF};
    dim_t dim;
  };

  std::vector<record_t> m_recs;
};

template<typename T>
LayoutRigid<T>::LayoutRigid(aplace_t aplc, layout_t layout)
    : LayoutMulti<T>(aplc)
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

template<typename T>
std::size_t LayoutRigid<T>::count_and_pad(layout_t& layout) const
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

}  // namespace display
