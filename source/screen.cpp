#include "display/screen.hpp"

namespace display
{

void Screen::resize(dim_t dim)
{
  m_dim = dim;
  if (m_layout != nullptr) {
    m_layout->resize(m_dim);
  }
}

void Screen::render(pos_t pos)  const
{
  if (m_layout != nullptr) {
    m_layout->render(pos);
  }
}

}  // namespace display
