#include <iostream>
#include <string>

#include "alec/alec.hpp"
#include "display/display.hpp"
#include "display/layout.hpp"

namespace
{

int renderer(display::Window& win)
{
  static int color_red = 0;

  std::cout << alec::background(color_red += 25, 65, 65);
  for (auto ypos = win.pos().y; ypos < win.pos().y + win.dim().height; ypos++) {
    std::cout << alec::cursor_position(ypos, win.pos().x);
    std::cout << std::string(win.dim().width, ' ');
  }
  std::cout << alec::background_v<alec::Color::DEFAULT>;
  std::cout << std::flush;

  return 0;
}

}  // namespace

int main()
{
  try {
    display::start();

    display::LayoutFree layout;
    layout.append({{3, 3}, {15, 15}});
    layout.append({{0, 0, 1}, {10, 10}});
    layout.append({{5, 5, 1}, {5, 10}});
    layout.append({{15, 15, 1}, {5, 10}});
    layout.render(renderer);

    while (true) {
      const auto event = alec::get_event();
      if (event.type() == alec::event::Type::KEY && event.key() == 'q') {
        break;
      }
    }

  } catch (std::exception& err) {
    std::cout << err.what() << '\n' << std::flush;
  }

  display::stop();

  return 0;
}
