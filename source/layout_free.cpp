#include "display/layout_free.hpp"

namespace display
{

void LayoutFree::render() const
{
  for (const auto& win : m_wins) {
    const auto plc = win->place(dim());

    if (!plc.has_value()) {
      continue;
    }

    win->render(apos() + plc.value());
  }
}

}  // namespace display
