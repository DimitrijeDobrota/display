#include <format>
#include <fstream>
#include <iostream>

#include "display/window.hpp"

namespace display
{

void Window::render() const
{
  const auto space = std::string(awth(), ' ');

  for (sz_t i = aypos(); i < aypos() + m_padd.top; i++) {
    set_cursor(i, axpos()) << space;
  }

  for (sz_t i = m_ypos; i < aypos() + ahgt(); i++) {
    set_cursor(i, axpos()) << space;
  }
  std::cout << std::flush;
}

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

std::ostream& Window::line_next() const
{
  if (m_ypos == ypos() + hgt()) {
    static std::ofstream null;
    null.setstate(std::ios_base::badbit);
    return null;
  }

  return set_cursor(m_ypos++, axpos());
}

void Window::line_empty() const
{
  line_next() << std::string(awth(), ' ');
}

void Window::line_left(const std::string& text) const
{
  const auto left = std::string(m_padd.left, ' ');
  const auto right = std::string(m_padd.right, ' ');

  line_next() << left << std::format("{:<{}}", text, wth()) << right;
}

void Window::line_center(const std::string& text) const
{
  const auto left = std::string(m_padd.left, ' ');
  const auto right = std::string(m_padd.right, ' ');

  line_next() << left << std::format("{:^{}}", text, wth()) << right;
}

void Window::line_right(const std::string& text) const
{
  const auto left = std::string(m_padd.left, ' ');
  const auto right = std::string(m_padd.right, ' ');

  line_next() << left << std::format("{:>{}}", text, wth()) << right;
}

}  // namespace display
