#include <format>
#include <fstream>
#include <iostream>

#include "display/window.hpp"

namespace display
{

void Window::clear() const
{
  std::cout << alec::background_v<alec::Color::DEFAULT>;
  std::cout << alec::foreground_v<alec::Color::DEFAULT>;

  for (sz_t i = 0; i < aypos() + ahgt(); i++) {
    set_cursor(i, axpos()) << std::string(awth(), ' ');
  }

  std::cout << std::flush;
}

void Window::line_reset() const
{
  m_ypos = ypos();
}

std::ostream& Window::set_cursor(sz_t posy, sz_t posx)
{
  return std::cout << alec::cursor_position(posy + 1, posx + 1);
}

std::ostream& Window::line_next() const
{
  if (m_ypos == ypos() + hgt()) {
    static std::ofstream null;
    null.setstate(std::ios_base::badbit);
    return null;
  }

  return set_cursor(m_ypos++, xpos());
}

void Window::line_empty() const
{
  line_next() << std::string(wth(), ' ');
}

void Window::line_left(const std::string& text) const
{
  line_next() << std::format("{:<{}}", text, wth());
}

void Window::line_center(const std::string& text) const
{
  line_next() << std::format("{:^{}}", text, wth());
}

void Window::line_right(const std::string& text) const
{
  line_next() << std::format("{:>{}}", text, wth());
}

}  // namespace display
