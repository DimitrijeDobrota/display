#include "display/window_pivot.hpp"

#include "display/utility.hpp"

namespace display
{

void WindowPivot::resize(apos_t apos, dim_t dim)
{
  const auto [cols, rows] = dim;
  const auto [posx, posy] = pos();

  if (posx > cols || posy > rows) {
    return;
  }

  const auto [wdth, hght] = this->dim();
  const display::sz_t zero = 0;
  const sz_t wdthm = wdth / 2;
  const sz_t hghtm = hght / 2;

  pos_t start;
  pos_t end;

  switch (piv().x) {
    case PvtX::Left:
      start.x = posx;
      end.x = add_lim(start.x, wdth, cols);
      break;
    case PvtX::Center:
      start.x = sub_lim(posx, wdthm, zero);
      end.x = add_lim(posx, wdthm, cols);
      break;
    case PvtX::Right:
      end.x = posx;
      start.x = sub_lim(end.x, wdth, zero);
      break;
  }

  switch (piv().y) {
    case PvtY::Top:
      start.y = posy;
      end.y = add_lim(start.y, hght, rows);
      break;
    case PvtY::Center:
      start.y = sub_lim(posy, hghtm, zero);
      end.y = add_lim(posy, hghtm, rows);
      break;
    case PvtY::Bottom:
      end.y = posy;
      start.y = sub_lim(end.y, hght, zero);
      break;
  }

  m_apos = apos + start;
  m_adim = end - start;
}

}  // namespace display
