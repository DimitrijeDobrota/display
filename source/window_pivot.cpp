#include "display/window_pivot.hpp"

#include "display/utility.hpp"

namespace display
{

plc_t WindowPivot::place(plc_t aplc, piv_t piv, dim_t dim)
{
  const auto [awth, ahth] = aplc.dim;
  const auto [wth, hgt] = dim;

  dim_t start(0, 0);
  dim_t end(0, 0);

  using display::add_lim, display::sub_lim;

  switch (piv.x) {
    case PvtX::Left:
      start.width = wth_t(0);
      end.width = add_lim(start.width, wth, awth);
      break;
    case PvtX::Center:
      start.width = sub_lim((awth / 2), (wth / 2), wth_t(0));
      end.width = add_lim(start.width, wth, awth);
      break;
    case PvtX::Right:
      end.width = awth;
      start.width = sub_lim(end.width, wth, wth_t(0));
      break;
  }

  switch (piv.y) {
    case PvtY::Top:
      start.height = hgt_t(0);
      end.height = add_lim(start.height, hgt, ahth);
      break;
    case PvtY::Center:
      start.height = sub_lim((ahth / 2), (hgt / 2), hgt_t(0));
      end.height = add_lim(start.height, hgt, ahth);
      break;
    case PvtY::Bottom:
      end.height = ahth;
      start.height = sub_lim(end.height, hgt, hgt_t(0));
      break;
  }

  return {aplc.pos + start, end - start};
}

}  // namespace display
