#include <iostream>
#include <string>

#include "alec/alec.hpp"
#include "display/display.hpp"
#include "display/layout.hpp"

namespace
{

int renderer(display::Window& win)
{
  using display::place_t;

  static int color_red = 0;
  color_red = (color_red + 25) % 256;

  const auto [start, end] = win.place().value_or(place_t());

  std::cout << alec::background(color_red, 65, 65);
  for (auto ypos = start.y; ypos <= end.y; ypos++) {
    std::cout << alec::cursor_position(ypos, start.x);
    std::cout << std::string(end.x - start.x + 1U, ' ');
  }
  std::cout << alec::background_v<alec::Color::DEFAULT>;
  std::cout << std::flush;

  return 0;
}

}  // namespace

int main()
{
  try {
    display::LayoutFree layout;

    auto redraw = [&]()
    {
      const auto [cols, rows] = alec::get_screen_size();
      const display::sz_t colsm = cols / 2;
      const display::sz_t rowm = rows / 2;

      layout[4].pos() = {0, 0};
      layout[5].pos() = {colsm, 0};
      layout[6].pos() = {cols, 0};
      layout[7].pos() = {cols, rowm};
      layout[0].pos() = {cols, rows};
      layout[1].pos() = {colsm, rows};
      layout[2].pos() = {0, rows};
      layout[3].pos() = {0, rowm};
      layout[8].pos() = {colsm, rowm};

      layout.render(renderer);
    };

    using display::PvtX, display::PvtY;

    layout.append({{}, {20, 10}, {PvtX::Left, PvtY::Top}});
    layout.append({{}, {20, 10}, {PvtX::Center, PvtY::Top}});
    layout.append({{}, {20, 10}, {PvtX::Right, PvtY::Top}});
    layout.append({{}, {20, 10}, {PvtX::Right, PvtY::Center}});
    layout.append({{}, {20, 10}, {PvtX::Right, PvtY::Bottom}});
    layout.append({{}, {20, 10}, {PvtX::Center, PvtY::Bottom}});
    layout.append({{}, {20, 10}, {PvtX::Left, PvtY::Bottom}});
    layout.append({{}, {20, 10}, {PvtX::Left, PvtY::Center}});
    layout.append({{}, {20, 10}, {PvtX::Center, PvtY::Center}});

    display::start();
    redraw();
    while (true) {
      const auto event = display::get_event();
      if (event.type() == display::event::Type::RESIZE) {
        std::cout << alec::erase_display_v<alec::Motion::WHOLE>;
        redraw();
        continue;
      }

      if (event.type() == display::event::Type::KEY && event.key() == 'q') {
        break;
      }
    }

  } catch (std::exception& err) {
    std::cout << err.what() << '\n' << std::flush;
  }

  display::stop();

  return 0;
}
