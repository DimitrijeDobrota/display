#include <format>
#include <iostream>

#include "display/window.hpp"

namespace display
{

std::ostream& Window::next_line(bool reset) const
{
  if (reset) {
    m_ypos = aypos();
  }
  return std::cout << alec::cursor_position(++m_ypos, axpos() + 1);
}

void Window::line_empty(bool reset) const
{
  next_line(reset) << std::string(awth(), ' ');
}

void Window::line_left(const std::string& text) const
{
  next_line() << std::format(" {:<{}} ", text, awth() - 2);
}

void Window::line_center(const std::string& text) const
{
  next_line() << std::format("{:^{}}", text, awth());
}

void Window::line_right(const std::string& text) const
{
  next_line() << std::format(" {:>{}} ", text, awth() - 2);
}

}  // namespace display
