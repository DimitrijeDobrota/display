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

void LayoutFree::render(pos_t pos) const
{
  for (const auto& win : m_wins) {
    const auto plc = win->place(this->dim());

    if (!plc.has_value()) {
      continue;
    }

    win->render(plc.value() + pos);
  }
}

}  // namespace display
