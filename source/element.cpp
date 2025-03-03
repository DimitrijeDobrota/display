#include <iostream>

#include "display/element.hpp"

namespace display
{

std::ostream& Element::set_cursor(xpos_t xapos, ypos_t yapos)
{
  return std::cout << alec::cursor_position(yapos.value() + 1,
                                            xapos.value() + 1);
}

std::ostream& Element::set_cursor(pos_t apos)
{
  return set_cursor(apos.x, apos.y);
}

void Element::clear() const
{
  std::cout << alec::background_v<alec::Color::DEFAULT>;
  std::cout << alec::foreground_v<alec::Color::DEFAULT>;

  for (auto j = ypos_t(0); j < aypos() + ahgt(); j++) {
    Element::set_cursor(axpos(), j) << std::string(awth().value(), ' ');
  }
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
}

}  // namespace display
