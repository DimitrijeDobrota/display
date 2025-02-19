#include <iostream>

#include "display/element.hpp"

namespace display
{

std::ostream& Element::set_cursor(sz_t posy, sz_t posx)
{
  return std::cout << alec::cursor_position(posy + 1, posx + 1);
}

void Element::render_border() const
{
  set_cursor(aypos(), axpos());

  std::cout << "┌";
  for (sz_t i = 2; i < awth(); i++) {
    std::cout << "─";
  }
  std::cout << "┐";

  for (sz_t i = aypos() + 1; i < aypos() + ahgt(); i++) {
    set_cursor(i, axpos()) << "│";
    set_cursor(i, axpos() + awth() - 1) << "│";
  }

  set_cursor(aypos() + ahgt() - 1, axpos());
  std::cout << "└";
  for (sz_t i = 2; i < awth(); i++) {
    std::cout << "─";
  }
  std::cout << "┘";

  std::cout << std::flush;
}

}  // namespace display
