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

  for (sz_t i = aypos() + ahgt() - m_padd.bottom; i < aypos() + ahgt(); i++) {
    set_cursor(i, axpos()) << space;
  }
}

void Window::render_border() const
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
