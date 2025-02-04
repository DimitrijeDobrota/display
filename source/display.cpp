#include <iostream>
#include <stdexcept>

#include "display/display.hpp"

#include <alec/alec.hpp>
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


  alec::init_buffer(2);
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
}

}  // namespace display
