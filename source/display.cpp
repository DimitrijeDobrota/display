#include <iostream>
#include <stdexcept>

#include "display/display.hpp"

#include <alec/alec.hpp>
#include <signal.h>
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

static int start_count = 0;  // NOLINT

void handle_sigwinch(int)
{
  const auto [col, row] = alec::get_screen_size();
  std::cout << col << " " << row << std::endl;
}

void exit()
{
  stop(/* force= */ true);
}

void start(exit_f f_exit)
{
  if (start_count++ > 0) {
    // already started
    return;
  }

  if (atexit(f_exit) != 0) {
    throw std::runtime_error("Can't register new exit function");
  }

  struct sigaction old_sig_action = {};
  sigaction(SIGWINCH, nullptr, &old_sig_action);
  if (old_sig_action.sa_handler != SIG_IGN) {
    struct sigaction new_sig_action = {};

    new_sig_action.sa_handler = handle_sigwinch;
    sigemptyset(&new_sig_action.sa_mask);
    new_sig_action.sa_flags = SA_RESTART;

    sigaction(SIGWINCH, &new_sig_action, nullptr);
  }

  alec::init_buffer(STDIN_FILENO);
  write<alec::abuf_enable_v>();
  write<alec::cursor_hide_v>();
}

void stop(bool force)
{
  if (!force && --start_count > 0) {
    // still need to keep open
    return;
  }

  alec::dest_buffer();
  write<alec::cursor_show_v>();
  write<alec::abuf_disable_v>();
}

}  // namespace display
