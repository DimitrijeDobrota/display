#include <iostream>
#include <string>

#include "alec/alec.hpp"
#include "display/display.hpp"
#include "display/layout.hpp"
#include "display/utility.hpp"

namespace
{

int renderer(display::Window& win)
{
  using display::add_lim, display::sub_lim;
  using display::PvtX, display::PvtY;
  using display::sz_t;

  static int color_red = 0;

  sz_t start_x = 0;
  sz_t end_x = 0;
  sz_t start_y = 0;
  sz_t end_y = 0;

  const auto [cols, rows] = alec::get_screen_size();
  const auto [posx, posy, _] = win.pos();
  const auto [wdth, hght] = win.dim();
  const display::sz_t zero = 0;
  const sz_t wdthm = wdth / 2;
  const sz_t hghtm = hght / 2;

  // Skip when pivot is outside the boundary
  if (posx < 0 || posx > cols) {
    return 0;
  }

  if (posy < 0 || posy > rows) {
    return 0;
  }

  switch (win.piv().x) {
    case PvtX::Left:
      start_x = posx;
      end_x = add_lim(start_x, wdth, cols);
      break;
    case PvtX::Center:
      start_x = sub_lim(posx, wdthm, zero);
      end_x = add_lim(posx, wdthm, cols);
      break;
    case PvtX::Right:
      end_x = posx;
      start_x = sub_lim(end_x, wdth, zero);
      break;
  }

  switch (win.piv().y) {
    case PvtY::Top:
      start_y = posy;
      end_y = add_lim(start_y, hght, rows);
      break;
    case PvtY::Center:
      start_y = sub_lim(posy, hghtm, zero);
      end_y = add_lim(posy, hghtm, rows);
      break;
    case PvtY::Bottom:
      end_y = posy;
      start_y = sub_lim(end_y, hght, zero);
      break;
  }

  color_red = (color_red + 25) % 256;
  std::cout << alec::background(color_red, 65, 65);
  for (auto ypos = start_y; ypos <= end_y; ypos++) {
    std::cout << alec::cursor_position(ypos, start_x);
    std::cout << std::string(end_x - start_x + 1, ' ');
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
