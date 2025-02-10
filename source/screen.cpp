#include "display/screen.hpp"

#include "display/layout.hpp"

namespace display
{

void Screen::set_layout(LayoutFree* layout)
{
  if (m_layout != nullptr) {
    m_layout->set_screen(nullptr);
  }

  m_layout = layout;

  if (m_layout != nullptr) {
    m_layout->set_screen(this);
  }
}

void Screen::resize(dim_t dim)
{
  m_dim = dim;
  if (m_layout != nullptr) {
    m_layout->recalc();
  }
}

}  // namespace display
