#include "display/layout_pivot.hpp"

namespace display
{

void LayoutPivot::resize(apos_t apos, dim_t dim)
{
  Layout::resize(apos, dim);

  if (has_window()) {
    m_window->pos() = get_pos();
    m_window->resize(apos, dim);
  }
}

void LayoutPivot::render() const
{
  if (has_window()) {
    m_window->render();
  }
}

void LayoutPivot::input(event& evnt)
{
  if (has_window()) {
    m_window->input(evnt);
  }
}

pos_t LayoutPivot::get_pos() const
{
  const auto [width, height] = dim();
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
