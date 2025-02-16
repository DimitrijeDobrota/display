#include "display/layout_pivot.hpp"

#include "display/window_pivot.hpp"

namespace display
{

void LayoutPivot::resize(aplace_t aplc)
{
  Layout::resize(aplc);

  if (has_child()) {
    get_child().set_pos(get_pos());
    get_child().resize(aplc);
  }
}

pos_t LayoutPivot::get_pos() const
{
  const auto [width, height] = adim();
  const display::sz_t midw = width / 2;
  const display::sz_t midh = height / 2;

  if (m_piv.x == PvtX::Left && m_piv.y == PvtY::Top) {
    return {0, 0};
  }

  if (m_piv.x == PvtX::Center && m_piv.y == PvtY::Top) {
    return {midw, 0};
  }

  if (m_piv.x == PvtX::Right && m_piv.y == PvtY::Top) {
    return {width, 0};
  }

  if (m_piv.x == PvtX::Right && m_piv.y == PvtY::Center) {
    return {width, midh};
  }

  if (m_piv.x == PvtX::Right && m_piv.y == PvtY::Bottom) {
    return {width, height};
  }

  if (m_piv.x == PvtX::Center && m_piv.y == PvtY::Bottom) {
    return {midw, height};
  }

  if (m_piv.x == PvtX::Left && m_piv.y == PvtY::Bottom) {
    return {0, height};
  }

  if (m_piv.x == PvtX::Left && m_piv.y == PvtY::Center) {
    return {0, midh};
  }

  if (m_piv.x == PvtX::Center && m_piv.y == PvtY::Center) {
    return {midw, midh};
  }

  return {0, 0};
}

}  // namespace display
