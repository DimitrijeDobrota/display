#include <iostream>

#include "display/window_padd.hpp"

namespace display
{

template<>
void WindowPadd<WindowType::Bare>::render() const
{
}

template<>
void WindowPadd<WindowType::Border>::render() const
{
  set_cursor(aypos(), axpos()) << std::string(awth(), ' ');

  for (sz_t i = ypos(); i < ypos() + hgt(); i++) {
    set_cursor(i, axpos()) << ' ';
    set_cursor(i, axpos() + awth() - 1) << ' ';
  }

  set_cursor(aypos() + ahgt() - 1, axpos()) << std::string(awth(), ' ');
}

template<>
void WindowPadd<WindowType::Box>::render() const
{
  set_cursor(aypos(), axpos());
  std::cout << "┌";
  for (sz_t i = 0; i < wth(); i++) {
    std::cout << "─";
  }
  std::cout << "┐";

  for (sz_t i = ypos(); i < ypos() + hgt(); i++) {
    set_cursor(i, axpos()) << "│";
    set_cursor(i, axpos() + awth() - 1) << "│";
  }

  set_cursor(aypos() + ahgt() - 1, axpos());
  std::cout << "└";
  for (sz_t i = 0; i < wth(); i++) {
    std::cout << "─";
  }
  std::cout << "┘";
}

template<>
void WindowPadd<WindowType::BorderBox>::render() const
{
  set_cursor(aypos(), axpos());
  std::cout << "┌─";
  for (sz_t i = 0; i < wth(); i++) {
    std::cout << "─";
  }
  std::cout << "─┐";

  for (sz_t i = ypos(); i < ypos() + hgt(); i++) {
    set_cursor(i, axpos()) << "│ ";
    set_cursor(i, axpos() + awth() - 2) << " │";
  }

  set_cursor(aypos() + ahgt() - 1, axpos());
  std::cout << "└─";
  for (sz_t i = 0; i < wth(); i++) {
    std::cout << "─";
  }
  std::cout << "─┘";
}

}  // namespace display
