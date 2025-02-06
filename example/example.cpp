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
  for (auto ypos = win.ypos(); ypos < win.ypos() + win.height(); ypos++) {
    std::cout << alec::cursor_position(ypos, win.xpos());
    std::cout << std::string(win.width(), ' ');
  }
  std::cout << alec::background_v<alec::Color::DEFAULT>;
  std::cout << std::flush;

  return 0;
}

}  // namespace

int main()
{
  display::start();

  display::LayoutFree layout;
  layout.add_window({3, 3, 15, 15});
  layout.add_window({0, 0, 10, 10}, 1);
  layout.add_window({5, 5, 5, 10}, 1);
  layout.render(renderer);

  while (true) {
    const auto event = alec::get_event();
    if (event.type() == alec::event::Type::KEY && event.key() == 'q') {
      break;
    }
  }

  display::stop();

  return 0;
}
