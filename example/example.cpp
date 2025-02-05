#include <format>
#include <iostream>

#include "alec/alec.hpp"
#include "display/display.hpp"

int main()
{
  display::start();

  while (true) {
    const auto event = alec::get_event();
    if (event.type() == alec::event::Type::KEY && event.key() == 'q') {
      break;
    }
  }

  display::stop();

  return 0;
}
