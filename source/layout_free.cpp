#include <algorithm>
#include <numeric>

#include "display/layout_free.hpp"

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
    idxs.resize(m_wins.size());
    std::iota(idxs.begin(), idxs.end(), 0);
    std::stable_sort(
        idxs.begin(),
        idxs.end(),
        [&](auto left, auto right)
        { return m_wins[left]->pos().z < m_wins[right]->pos().z; });
    m_is_sorted = true;
  }

  for (const auto idx : idxs) {
    const auto& win = m_wins[idx];
    const auto plc = win->place(this->dim());

    if (!plc.has_value()) {
      continue;
    }

    win->render(plc.value() + pos);
  }

  return 0;
}
}  // namespace display
