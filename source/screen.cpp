#include "display/screen.hpp"

namespace display
{

void Screen::resize(apos_t apos, dim_t dim)
{
  m_apos = apos;
  m_dim = dim;

  if (has_layout()) {
    m_layout->resize(apos, m_dim);
  }
}

void Screen::render() const
{
  if (has_layout()) {
    m_layout->render();
  }
}

}  // namespace display
