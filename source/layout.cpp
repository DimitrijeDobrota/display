#include <algorithm>
#include <numeric>

#include "display/layout.hpp"

namespace display
{

void LayoutFree::append(Window window, int zidx)
{
  m_windows.emplace_back(zidx, std::move(window));
  m_is_sorted = false;
}

int LayoutFree::render(render_f renderer)
{
  static std::vector<std::uint8_t> idxs;

  if (!m_is_sorted) {
    idxs.resize(m_windows.size());
    std::iota(idxs.begin(), idxs.end(), 0);
    std::stable_sort(idxs.begin(),  // NOLINT
                     idxs.end(),
                     [&](auto left, auto right)
                     {
                       return std::get<0>(m_windows[left])
                           < std::get<0>(m_windows[right]);
                     });
    m_is_sorted = true;
  }

  for (const auto idx : idxs) {
    const auto res = renderer(std::get<1>(m_windows[idx]));
    if (res != 0) {
      return res;
    }
  }

  return 0;
}

}  // namespace display
