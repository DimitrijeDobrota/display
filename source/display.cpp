#include <csignal>

#include "display/display.hpp"

#include <termios.h>
#include <unistd.h>

namespace
{

template<const char* Val>
inline void write()
{
  ::write(STDIN_FILENO, Val, sizeof(Val));
}

}  // namespace

namespace display
{

bool Display::is_resize_track = false;

Display& Display::display()
{
  static Display instance;
  return instance;
}

Display::Display()
    : m_layout(place_t(pos_t(0, 0), alec::get_screen_size()))
{
  struct sigaction old_sig_action = {};
  sigaction(SIGWINCH, nullptr, &old_sig_action);
  if (old_sig_action.sa_handler != SIG_IGN) {
    struct sigaction new_sig_action = {};

    new_sig_action.sa_handler = handle_sigwinch;
    sigemptyset(&new_sig_action.sa_mask);
    new_sig_action.sa_flags = SA_RESTART;

    sigaction(SIGWINCH, &new_sig_action, nullptr);
    is_resize_track = true;
  }

  alec::init_buffer(STDIN_FILENO);
  write<alec::abuf_enable_v>();
  write<alec::cursor_hide_v>();

  resize();
}

Display::~Display()
{
  alec::dest_buffer();
  write<alec::cursor_show_v>();
  write<alec::abuf_disable_v>();
}

void Display::handle_sigwinch(int /* unused */)
{
  Display::display().set_resized();
}

event Display::get_event()  // NOLINT
{
  if (is_resize_track && m_is_resized) {
    Display::reset_resized();
    return {event::Type::RESIZE, 0, 0};
  }

  return alec::get_event();
}

void Display::set_resized()
{
  m_is_resized = true;
  resize();
}

void Display::reset_resized()
{
  m_is_resized = false;
}

bool Display::get_resized() const
{
  return m_is_resized;
}

void Display::resize()
{
  m_layout.resize(place_t(pos_t(0, 0), alec::get_screen_size()));
}

void Display::render() const
{
  m_layout.render();
}

void Display::input(event& evnt)
{
  m_layout.input(evnt);
}

}  // namespace display
