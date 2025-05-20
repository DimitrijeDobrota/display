#include <format>
#include <fstream>
#include <iostream>

#include "display/window.hpp"

namespace display
{

void Window::render() const
{
  const auto space = std::string(awth().value(), ' ');

  for (auto j = aypos(); j < aypos() + m_padd.top; j++) {
    set_cursor(axpos(), j) << space;
  }

  for (auto j = m_ypos; j < aypos() + ahgt(); j++) {
    set_cursor(axpos(), j) << space;
  }
  std::cout << std::flush;
}

void Window::clear() const
{
  std::cout << alec::background_v<alec::color::def>;
  std::cout << alec::foreground_v<alec::color::def>;

  for (auto j = ypos_t(0); j < aypos() + ahgt(); j++) {
    set_cursor(axpos(), j) << std::string(awth().value(), ' ');
  }

  std::cout << std::flush;
}

void Window::line_reset() const
{
  m_ypos = ypos();
}

std::ostream& Window::line_next() const
{
  if (m_ypos == ypos() + hgt()) {
    static std::ofstream null;
    null.setstate(std::ios_base::badbit);
    return null;
  }

  return set_cursor(axpos(), m_ypos++);
}

void Window::line_empty() const
{
  line_next() << std::string(awth().value(), ' ');
}

void Window::line_left(const std::string& text) const
{
  const auto left = std::string(m_padd.left.value(), ' ');
  const auto right = std::string(m_padd.right.value(), ' ');

  line_next() << left << std::format("{:<{}}", text, wth().value()) << right;
}

void Window::line_center(const std::string& text) const
{
  const auto left = std::string(m_padd.left.value(), ' ');
  const auto right = std::string(m_padd.right.value(), ' ');

  line_next() << left << std::format("{:^{}}", text, wth().value()) << right;
}

void Window::line_right(const std::string& text) const
{
  const auto left = std::string(m_padd.left.value(), ' ');
  const auto right = std::string(m_padd.right.value(), ' ');

  line_next() << left << std::format("{:>{}}", text, wth().value()) << right;
}

}  // namespace display
