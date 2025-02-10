#include <algorithm>
#include <numeric>

#include "display/layout.hpp"

#include "display/screen.hpp"
#include "display/window.hpp"

namespace display
{

dim_t LayoutFree::dim() const
{
  return m_screen != nullptr ? m_screen->dim() : dim_t();
}

void LayoutFree::append(Window window)
{
  m_windows.push_back(window);
  m_windows.back().set_layout(this);
  m_is_sorted = false;
}

void LayoutFree::recalc()
{
  m_recalc(*this);
}

int LayoutFree::render(render_f renderer)
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
    const auto res = renderer(m_windows[idx]);
    if (res != 0) {
      return res;
    }
  }

  return 0;
}

}  // namespace display
