#include <iostream>
#include <string>

#include "alec/alec.hpp"
#include "display/display.hpp"
#include "display/layout.hpp"

namespace
{

int renderer(display::Window& win)
{
  using display::PvtX;
  using display::PvtY;

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

  std::cout << alec::background(color_red += 25, 65, 65);
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
    display::start();

    const auto [cols, rows] = alec::get_screen_size();
    const std::uint16_t colsm = cols / 2;
    const std::uint16_t rowm = rows / 2;

    using display::PvtX;
    using display::PvtY;

    display::LayoutFree layout;
    layout.append({{0, 0}, {20, 10}, {PvtX::Left, PvtY::Top}});
    layout.append({{colsm, 0}, {20, 10}, {PvtX::Center, PvtY::Top}});
    layout.append({{cols, 0}, {20, 10}, {PvtX::Right, PvtY::Top}});
    layout.append({{cols, rowm}, {20, 10}, {PvtX::Right, PvtY::Center}});
    layout.append({{cols, rows}, {20, 10}, {PvtX::Right, PvtY::Bottom}});
    layout.append({{colsm, rows}, {20, 10}, {PvtX::Center, PvtY::Bottom}});
    layout.append({{0, rows}, {20, 10}, {PvtX::Left, PvtY::Bottom}});
    layout.append({{0, rowm}, {20, 10}, {PvtX::Left, PvtY::Center}});
    layout.append({{colsm, rowm}, {20, 10}, {PvtX::Center, PvtY::Center}});
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
