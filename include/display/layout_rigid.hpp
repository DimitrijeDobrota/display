#pragma once

#include <stdexcept>
#include <unordered_set>
#include <vector>

#include <based/types/types.hpp>

#include "display/layout.hpp"
#include "display/types.hpp"

namespace display
{

using namespace literals;  // NOLINT(*namespace*)

template<typename T = Element>
class LayoutRigid : public LayoutMulti<T>
{
public:
  using layout_t = std::vector<std::vector<based::bu8>>;

  LayoutRigid(plc_t aplc, layout_t layout);

  template<typename M = T, class... Args>
    requires(std::is_base_of_v<T, M>)
  M& append(Args&&... args)
  {
    return LayoutMulti<T>::template append<M>(
        place(this->size()), std::forward<Args>(args)...
    );
  }

  void resize(plc_t aplc) override
  {
    LayoutMulti<T>::resize(aplc);

    for (std::size_t i = 0; i < this->size(); i++) {
      this->get(i).resize(place(i));
    }
  }

protected:
  [[nodiscard]] plc_t place(std::size_t idx) const
  {
    const auto [m, n] = m_grid;
    const auto [w, h] = this->adim();
    const auto unw = w / m;
    const auto unh = h / n;

    const auto calc = [&]<typename R>(const R share, bool addw, bool addh) -> R
    {
      const auto [sw, sh] = share;

      const auto wth =
          addw ? w - (unw * (m - wth_t(sw.value))) : unw * wth_t(sw.value);
      const auto hgt =
          addh ? h - (unh * (n - hgt_t(sh.value))) : unh * hgt_t(sh.value);

      if constexpr (std::is_same_v<R, pos_t>) {
        return {xpos_t(wth.value), ypos_t(hgt.value)};
      } else {
        return {wth, hgt};
      }
    };

    const auto start = calc(m_recs[idx].start, false, false);
    const auto dim = calc(m_recs[idx].dim, m_recs[idx].addw, m_recs[idx].addh);
    return {this->apos() + start, dim};
  }

  [[nodiscard]] auto get_grid() const { return m_grid; }
  [[nodiscard]] const auto& get_record(std::size_t idx) const
  {
    return m_recs[idx];
  }

  struct record_t
  {
    pos_t start = {0_x, 0_y};
    dim_t dim = {0_w, 0_h};
    bool addw = false;
    bool addh = false;
  };

private:
  std::size_t count_and_pad(layout_t& layout) const;
  void handle_cols(const layout_t& layout);
  void handle_rows(const layout_t& layout);

  dim_t m_grid;
  std::vector<record_t> m_recs;
};

template<typename T>
LayoutRigid<T>::LayoutRigid(plc_t aplc, layout_t layout)
    : LayoutMulti<T>(aplc)
    , m_grid(wth_t(layout[0].size()), hgt_t(layout.size()))
    , m_recs(count_and_pad(layout))
{
  handle_cols(layout);
  handle_rows(layout);
}

template<typename T>
std::size_t LayoutRigid<T>::count_and_pad(layout_t& layout) const
{
  std::unordered_set<based::bu8> ust;
  static constexpr based::bu8 padd = 0xFF;

  for (std::size_t i = 0U; i < m_grid.height.value; i++) {
    for (std::size_t j = 0U; j < m_grid.width.value; j++) {
      ust.insert(layout[i][j]);
    }
    layout[i].emplace_back(padd);
  }
  layout.emplace_back(m_grid.width.value, padd);

  for (std::size_t i = 0U; i < m_grid.height.value; i++) {
    for (std::size_t j = 0U; j < m_grid.width.value; j++) {
      if (layout[i][j] >= ust.size()) {
        throw std::runtime_error("Invalid layout [Number]");
      }
    }
  }

  return ust.size();
}

template<typename T>
void LayoutRigid<T>::handle_cols(const layout_t& layout)
{
  const auto [m, n] = get_grid();

  for (auto i = 0_h; i < n; ++i) {
    m_recs[layout[i.value][m.value - 1]].addw = true;

    auto cnt = 1_w;
    for (auto j = 0_w; j < m; ++j) {
      const auto crnt = layout[i.value][j.value];

      if (crnt == layout[i.value][j.value + 1]) {
        ++cnt;
        continue;
      }

      auto& count = m_recs[crnt].dim.width;
      auto& pos = m_recs[crnt].start.x;
      const auto total = xpos_t() + j - cnt + 1_w;

      if (count.value != 0) {
        if (pos != total || count != cnt) {
          throw std::runtime_error("Invalid layout [Shape Col]");
        }
      } else {
        pos = total;
        count = cnt;
      }

      cnt = 1_w;
    }
  }
}

template<typename T>
void LayoutRigid<T>::handle_rows(const layout_t& layout)
{
  const auto [m, n] = get_grid();

  for (auto j = 0_w; j < m; ++j) {
    m_recs[layout[n.value - 1][j.value]].addh = true;

    auto cnt = 1_h;
    for (auto i = 0_h; i < n; ++i) {
      const auto crnt = layout[i.value][j.value];

      if (crnt == layout[i.value + 1][j.value]) {
        ++cnt;
        continue;
      }

      auto& count = m_recs[crnt].dim.height;
      auto& pos = m_recs[crnt].start.y;
      const auto total = ypos_t() + i - cnt + 1_h;

      if (count.value != 0) {
        if (pos != total || count != cnt) {
          throw std::runtime_error("Invalid layout [Shape Row]");
        }
      } else {
        pos = total;
        count = cnt;
      }

      cnt = 1_h;
    }
  }
}

}  // namespace display
