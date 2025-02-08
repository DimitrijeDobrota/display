#include <iostream>
#include <string>

#include "alec/alec.hpp"
#include "display/display.hpp"
#include "display/layout.hpp"

namespace
{

int renderer(display::Window& win)
{
  using display::PvtX, display::PvtY;

  static int color_red = 0;

  display::sz_t start_x = 0;
  display::sz_t end_x = 0;
  display::sz_t start_y = 0;
  display::sz_t end_y = 0;

  switch (win.piv().x) {
    case PvtX::Left:
      start_x = win.pos().x;
      end_x = start_x + win.dim().width;
      break;
    case PvtX::Center:
      start_x = win.pos().x - win.dim().width / 2;
      end_x = start_x + win.dim().width;
      break;
    case PvtX::Right:
      end_x = win.pos().x;
      start_x = end_x - win.dim().width;
      break;
  }

  switch (win.piv().y) {
    case PvtY::Top:
      start_y = win.pos().y;
      end_y = start_y + win.dim().height;
      break;
    case PvtY::Center:
      start_y = win.pos().y - win.dim().height / 2;
      end_y = start_y + win.dim().height;
      break;
    case PvtY::Bottom:
      end_y = win.pos().y;
      start_y = end_y - win.dim().height;
      break;
  }

  color_red = (color_red + 25) % 256;
  std::cout << alec::background(color_red, 65, 65);
  for (auto ypos = start_y; ypos <= end_y; ypos++) {
    std::cout << alec::cursor_position(ypos, start_x);
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
    display::LayoutFree layout;

    auto redraw = [&]()
    {
      const auto [cols, rows] = alec::get_screen_size();
      const std::uint16_t colsm = cols / 2;
      const std::uint16_t rowm = rows / 2;

      layout[0].pos() = {0, 0};
      layout[1].pos() = {colsm, 0};
      layout[2].pos() = {cols, 0};
      layout[3].pos() = {cols, rowm};
      layout[4].pos() = {cols, rows};
      layout[5].pos() = {colsm, rows};
      layout[6].pos() = {0, rows};
      layout[7].pos() = {0, rowm};
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
