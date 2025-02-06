#include <algorithm>

#include "display/layout.hpp"

namespace display
{

void LayoutFree::add_window(Window window, int zidx)
{
  m_windows.emplace_back(zidx, std::move(window));
  m_is_sorted = false;
}

int LayoutFree::render(render_f renderer)
{
  if (!m_is_sorted) {
    std::stable_sort(m_windows.begin(),  // NOLINT
                     m_windows.end(),
                     [](auto& fst, auto& sec)
                     { return std::get<0>(fst) < std::get<0>(sec); });
    m_is_sorted = true;
  }

  for (auto& [_, window] : m_windows) {
    const auto res = renderer(window);
    if (res != 0) {
      return res;
    }
  }

  return 0;
}

}  // namespace display
