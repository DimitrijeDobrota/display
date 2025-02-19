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

  LayoutRigid(place_t aplc, layout_t layout);

  template<typename M = T, class... Args>
    requires(std::is_base_of_v<T, M>)
  M& append(Args&&... args)
  {
    return LayoutMulti<T>::template append<M>(place(this->size()),
                                              std::forward<Args>(args)...);
  }

  void resize(place_t aplc) override
  {
    LayoutMulti<T>::resize(aplc);

    for (std::size_t i = 0; i < this->size(); i++) {
      this->get(i).resize(place(i));
    }
  }

protected:
  place_t place(std::size_t idx) const
  {
    const auto [m, n] = m_grid;
    const auto [w, h] = this->adim();
    const sz_t unw = w / m;
    const sz_t unh = h / n;

    const auto calc = [&]<typename R>(const R share, bool addw, bool addh) -> R
    {
      const auto [sw, sh] = share;

      const sz_t width = addw ? w - (unw * (m - sw)) : unw * sw;
      const sz_t height = addh ? h - (unh * (n - sh)) : unh * sh;

      return {width, height};
    };

    const auto start = calc(m_recs[idx].start, false, false);
    const auto dim = calc(m_recs[idx].dim, m_recs[idx].addw, m_recs[idx].addh);
    return {this->apos() + start, dim};
  }

  const auto& get_record(std::size_t idx) { return m_recs[idx]; }

  struct record_t
  {
    pos_t start;
    dim_t dim;
    bool addw = false;
    bool addh = false;
  };

private:
  std::size_t count_and_pad(layout_t& layout) const;

  dim_t m_grid;
  std::vector<record_t> m_recs;
};

template<typename T>
LayoutRigid<T>::LayoutRigid(place_t aplc, layout_t layout)
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
    uint8_t cnt = 1;

    m_recs[layout[i][m_grid.width - 1]].addw = true;
    for (std::size_t j = 0U; j < m_grid.width; j++) {
      const auto crnt = layout[i][j];

      if (crnt == layout[i][j + 1]) {
        cnt++;
        continue;
      }

      insert(m_recs[crnt].dim.width, cnt, m_recs[crnt].start.x, j - cnt + 1);
      cnt = 1;
    }
  }

  for (std::size_t j = 0U; j < m_grid.width; j++) {
    uint8_t cnt = 1;

    m_recs[layout[m_grid.height - 1][j]].addh = true;
    for (std::size_t i = 0U; i < m_grid.height; i++) {
      const auto crnt = layout[i][j];

      if (crnt == layout[i + 1][j]) {
        cnt++;
        continue;
      }

      insert(m_recs[crnt].dim.height, cnt, m_recs[crnt].start.y, i - cnt + 1);
      cnt = 1;
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
