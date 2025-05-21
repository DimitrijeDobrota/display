#include "display/window_pivot.hpp"

#include "display/utility.hpp"

namespace display
{

using namespace literals;  // NOLINT(*namespace*)

plc_t WindowPivot::place(plc_t aplc, piv_t piv, dim_t dim)
{
  const auto [awth, ahth] = aplc.dim;
  const auto [wth, hgt] = dim;

  dim_t start(0_w, 0_h);
  dim_t end(0_w, 0_h);

  using display::add_lim, display::sub_lim;

  switch (piv.piv_x()) {
    case piv_t::x::left():
      start.width = 0_w;
      end.width = add_lim(start.width, wth, awth);
      break;
    case piv_t::x::center():
      start.width = sub_lim((awth / 2_w), (wth / 2_w), 0_w);
      end.width = add_lim(start.width, wth, awth);
      break;
    case piv_t::x::right():
      end.width = awth;
      start.width = sub_lim(end.width, wth, 0_w);
      break;
  }

  switch (piv.piv_y()) {
    case piv_t::y::top():
      start.height = 0_h;
      end.height = add_lim(start.height, hgt, ahth);
      break;
    case piv_t::y::center():
      start.height = sub_lim((ahth / 2_h), (hgt / 2_h), 0_h);
      end.height = add_lim(start.height, hgt, ahth);
      break;
    case piv_t::y::bottom():
      end.height = ahth;
      start.height = sub_lim(end.height, hgt, 0_h);
      break;
  }

  return {aplc.pos + start, end - start};
}

}  // namespace display
