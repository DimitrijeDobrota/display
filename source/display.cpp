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
static struct termios orig_termios;  // NOLINT

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

  if (isatty(STDIN_FILENO) == 0) {
    throw std::runtime_error("STDIN_FILENO is not associated with a terminal");
  }

  if (atexit(f_exit) != 0) {
    throw std::runtime_error("Can't register new exit function");
  }

  if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) {
    throw std::runtime_error("Can't read termios");
  }

  struct termios raw = orig_termios;

  // NOLINTBEGIN
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN);  // | ISIG
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;
  // NOLINTEND

  /* put terminal in raw mode after flushing */
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) < 0) {
    throw std::runtime_error("Can't write termios");
  }

  write<alec::abuf_enable_v>();
  write<alec::cursor_hide_v>();
}

void stop(bool force)
{
  if (!force && --start_count > 0) {
    // still need to keep open
    return;
  }

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) < 0) {
    throw std::runtime_error("Can't write termios");
  }

  write<alec::cursor_show_v>();
}

}  // namespace display
