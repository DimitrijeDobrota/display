#include <algorithm>
#include <numeric>

#include "display/layout.hpp"

#include "display/window.hpp"

namespace display
{

void LayoutFree::append(Window window)
{
  m_windows.push_back(window);
  m_is_sorted = false;
}

void LayoutFree::resize(pos_t pos, dim_t dim)
{
  m_pos = pos;
  m_dim = dim;
  m_recalc(*this);
}

int LayoutFree::render() const
{
  static std::vector<std::uint8_t> idxs;

  if (!m_is_sorted) {
    idxs.resize(m_windows.size());
    std::iota(idxs.begin(), idxs.end(), 0);
    std::stable_sort(
        idxs.begin(),
        idxs.end(),
        [&](auto left, auto right)
        { return m_windows[left].pos().z < m_windows[right].pos().z; });
    m_is_sorted = true;
  }

  for (const auto idx : idxs) {
    const auto win = m_windows[idx];
    const auto plc = win.place(m_dim);

    if (!plc.has_value()) {
      continue;
    }

    const auto res = win.render(plc.value());
    if (res != 0) {
      return res;
    }
  }

  return 0;
}

}  // namespace display
