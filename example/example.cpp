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

    const auto size = alec::get_screen_size();
    std::cout << std::format("({}, {})\n", std::get<0>(size), std::get<1>(size))
              << std::flush;
  }

  display::stop();

  return 0;
}
