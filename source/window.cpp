#include <format>
#include <fstream>
#include <iostream>

#include "display/window.hpp"

namespace display
{

std::ostream& Window::set_cursor(xpos_t xpos, ypos_t ypos) const
{
  return Element::set_cursor(axpos() + xpos, aypos() + ypos);
}

std::ostream& Window::set_cursor(pos_t pos) const
{
  return Element::set_cursor(apos() + pos);
}

void Window::render() const
{
  const auto space = std::string(awth().value(), ' ');

  for (auto j = aypos(); j < aypos() + m_padd.top; j++) {
    Element::set_cursor(axpos(), j) << space;
  }

  for (auto j = m_ypos; j < aypos() + ahgt(); j++) {
    Element::set_cursor(axpos(), j) << space;
  }
}

void Window::clear() const
{
  std::cout << alec::background_v<alec::Color::DEFAULT>;
  std::cout << alec::foreground_v<alec::Color::DEFAULT>;

  for (auto j = ypos_t(0); j < aypos() + ahgt(); j++) {
    Element::set_cursor(axpos(), j) << std::string(awth().value(), ' ');
  }
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

  return Element::set_cursor(axpos(), m_ypos++);
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
