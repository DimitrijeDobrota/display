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

void recalculator(display::LayoutFree& layout)
{
  const auto [width, height] = layout.dim();
  const display::sz_t midw = width / 2;
  const display::sz_t midh = height / 2;

  layout[4].pos() = {0, 0};
  layout[5].pos() = {midw, 0};
  layout[6].pos() = {width, 0};
  layout[7].pos() = {width, midh};
  layout[0].pos() = {width, height};
  layout[1].pos() = {midw, height};
  layout[2].pos() = {0, height};
  layout[3].pos() = {0, midh};
  layout[8].pos() = {midw, midh};
}

}  // namespace

int main()
{
  try {
    using display::Display;
    using display::PvtX, display::PvtY;

    auto& disp = Display::display();

    display::LayoutFree layout(recalculator);
    disp.screen().set_layout(&layout);

    layout.append({{}, {20, 10}, {PvtX::Left, PvtY::Top}});
    layout.append({{}, {20, 10}, {PvtX::Center, PvtY::Top}});
    layout.append({{}, {20, 10}, {PvtX::Right, PvtY::Top}});
    layout.append({{}, {20, 10}, {PvtX::Right, PvtY::Center}});
    layout.append({{}, {20, 10}, {PvtX::Right, PvtY::Bottom}});
    layout.append({{}, {20, 10}, {PvtX::Center, PvtY::Bottom}});
    layout.append({{}, {20, 10}, {PvtX::Left, PvtY::Bottom}});
    layout.append({{}, {20, 10}, {PvtX::Left, PvtY::Center}});
    layout.append({{}, {20, 10}, {PvtX::Center, PvtY::Center}});

    for (disp.set_resized(); true;) {
      using display::event;

      const auto evnt = disp.get_event();
      if (evnt.type() == event::Type::RESIZE) {
        std::cout << alec::erase_display_v<alec::Motion::WHOLE>;
        layout.render(renderer);
        continue;
      }

      if (evnt.type() == event::Type::KEY && evnt.key() == 'q') {
        break;
      }
    }

  } catch (std::exception& err) {
    std::cout << err.what() << '\n' << std::flush;
  }

  return 0;
}
