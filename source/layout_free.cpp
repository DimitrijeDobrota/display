#include "display/layout_free.hpp"

namespace display
{

void LayoutFree::resize(apos_t apos, dim_t dim)
{
  for (const auto& win : m_wins) {
    win->resize(apos, dim);
  }
}

void LayoutFree::render() const
{
  for (const auto& win : m_wins) {
    win->render();
  }
}

void LayoutFree::input(event& evnt)
{
  for (const auto& win : m_wins) {
    win->input(evnt);
    if (evnt.type() == event::Type::NONE) {
      break;
    }
  }
}

}  // namespace display
