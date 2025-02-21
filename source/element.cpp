#include <iostream>

#include "display/element.hpp"

namespace display
{

std::ostream& Element::set_cursor(xpos_t xpos, ypos_t ypos)
{
  return std::cout << alec::cursor_position(ypos.value() + 1, xpos.value() + 1);
}

std::ostream& Element::set_cursor(pos_t pos)
{
  return set_cursor(pos.x, pos.y);
}

void Element::render_border() const
{
  set_cursor(axpos(), aypos());

  std::cout << "┌";
  for (auto i = wth_t(2); i < awth(); i++) {
    std::cout << "─";
  }
  std::cout << "┐";

  for (ypos_t j = aypos() + 1; j < aypos() + ahgt(); j++) {
    set_cursor(axpos(), j) << "│";
    set_cursor(axpos() + awth() - 1, j) << "│";
  }

  set_cursor(axpos(), aypos() + ahgt() - 1);
  std::cout << "└";
  for (auto i = wth_t(2); i < awth(); i++) {
    std::cout << "─";
  }
  std::cout << "┘";

  std::cout << std::flush;
}

}  // namespace display
