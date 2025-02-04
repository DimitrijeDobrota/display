#include <iostream>

#include "alec/alec.hpp"
#include "display/display.hpp"

int main()
{
  display::start();

  while (true) {
    const auto event = alec::get_event();
    if (event.type() == alec::event::Type::NONE) {
      continue;
    }

    if (event.type() == alec::event::Type::KEY && event.key() == 'q') {
      break;
    }
    std::cout << char(event.key()) << std::flush;
  }

  display::stop();

  return 0;
}
