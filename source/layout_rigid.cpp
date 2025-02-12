#include "display/layout_rigid.hpp"

namespace display
{

void LayoutRigid::resize(dim_t dim)
{
  this->dim() = dim;

  const sz_t mid = this->dim().height / 2;
  m_screen1.resize({dim.width, mid});
  m_screen2.resize({dim.width, mid});

  if (m_recalc != nullptr) {
    m_recalc(*this);
  }
}

int LayoutRigid::render(pos_t pos) const
{
  const sz_t mid = this->dim().height / 2;
  m_screen1.render(pos + pos_t(0, 0));
  m_screen2.render(pos + pos_t(0, mid + 1));
  return 0;
}

}  // namespace display
