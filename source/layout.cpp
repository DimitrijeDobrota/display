#include <algorithm>
#include <numeric>

#include "display/layout.hpp"

#include "display/window.hpp"

namespace display
{

void LayoutFree::resize(dim_t dim)
{
  this->dim() = dim;

  if (m_recalc != nullptr) {
    m_recalc(*this);
  }
}

int LayoutFree::render(pos_t pos) const
{
  static std::vector<std::uint8_t> idxs;

  if (!m_is_sorted) {
    idxs.resize(m_windows.size());
    std::iota(idxs.begin(), idxs.end(), 0);
    std::stable_sort(
        idxs.begin(),
        idxs.end(),
        [&](auto left, auto right)
        { return m_windows[left]->pos().z < m_windows[right]->pos().z; });
    m_is_sorted = true;
  }

  for (const auto idx : idxs) {
    const auto& win = m_windows[idx];
    const auto plc = win->place(this->dim());

    if (!plc.has_value()) {
      continue;
    }

    win->render(plc.value() + pos);
  }

  return 0;
}

void LayoutRigid::resize(dim_t dim)
{
  this->dim() = dim;

  const sz_t mid = this->dim().height / 2;
  m_screen1.resize({dim.width, mid});
  m_screen2.resize({dim.width, mid});

  if (m_recalc != nullptr) {
    m_recalc(*this);
  }
}

int LayoutRigid::render(pos_t pos) const
{
  const sz_t mid = this->dim().height / 2;
  m_screen1.render(pos + pos_t(0, 0));
  m_screen2.render(pos + pos_t(0, mid + 1));
  return 0;
}

}  // namespace display
