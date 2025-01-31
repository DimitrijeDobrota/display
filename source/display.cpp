#include <iostream>

#include "display/display.hpp"

#include <alec/alec.hpp>

namespace display
{

void start()
{
  std::cout << alec::abuf_enable_v << alec::cursor_hide_v;
}

void stop()
{
  std::cout << alec::abuf_disable_v;
}

}  // namespace display
