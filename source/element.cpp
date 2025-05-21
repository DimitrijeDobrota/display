#include <iostream>

#include "display/element.hpp"

namespace display
{

std::ostream& Element::set_cursor(xpos_t xpos, ypos_t ypos)
{
  return std::cout << alec::cursor_position(ypos.value + 1, xpos.value + 1);
}

std::ostream& Element::set_cursor(pos_t pos)
{
  return set_cursor(pos.x, pos.y);
}

void Element::render_border() const
{
  using namespace literals;  // NOLINT(*namespace*)
  set_cursor(axpos(), aypos());

  std::cout << "┌";
  for (auto i = 2_w; i < awth(); ++i) {
    std::cout << "─";
  }
  std::cout << "┐";

  for (auto j = aypos() + 1_y; j < aypos() + ahgt(); ++j) {
    set_cursor(axpos(), j) << "│";
    set_cursor(axpos() + awth() - 1_w, j) << "│";
  }

  set_cursor(axpos(), aypos() + ahgt() - 1_h);
  std::cout << "└";
  for (auto i = 2_w; i < awth(); ++i) {
    std::cout << "─";
  }
  std::cout << "┘";

  std::cout << std::flush;
}

}  // namespace display
