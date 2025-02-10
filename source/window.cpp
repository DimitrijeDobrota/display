#include "display/window.hpp"

#include "display/utility.hpp"

namespace display
{

int Window::render(place_t place) const
{
  return m_renderer(*this, place);
}

std::optional<place_t> Window::place(dim_t bounds) const
{
  const auto [cols, rows] = bounds;
  const auto [posx, posy, _] = pos();

  if (posx > cols || posy > rows) {
    return {};
  }

  const auto [wdth, hght] = dim();
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

  return place_t(start, end);
}

}  // namespace display
