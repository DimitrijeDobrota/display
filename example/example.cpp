#include <iostream>
#include <string>

#include "alec/alec.hpp"
#include "display/display.hpp"
#include "display/layout.hpp"

namespace
{

void renderer(const display::Window& win, display::place_t plc)
{
  using display::place_t;

  static int color_red = 0;
  color_red = (color_red + 25) % 256;

  const auto [start, end] = plc;

  std::cout << alec::background(color_red, 65, 65);
  for (auto ypos = start.y; ypos <= end.y; ypos++) {
    std::cout << alec::cursor_position(ypos, start.x);
    std::cout << std::string(end.x - start.x + 1U, ' ');
  }
  std::cout << alec::background_v<alec::Color::DEFAULT>;
  std::cout << std::flush;

  (void)win;
}

void recalculator1(display::LayoutFree& layout)
{
  const auto [width, height] = layout.dim();
  const display::sz_t midw = width / 2;
  const display::sz_t midh = height / 2;

  layout[4]->pos() = {0, 0};
  layout[5]->pos() = {midw, 0};
  layout[6]->pos() = {width, 0};
  layout[7]->pos() = {width, midh};
  layout[0]->pos() = {width, height};
  layout[1]->pos() = {midw, height};
  layout[2]->pos() = {0, height};
  layout[3]->pos() = {0, midh};
  layout[8]->pos() = {midw, midh};
}

void recalculator2(display::LayoutFree& layout)
{
  const auto [width, height] = layout.dim();
  const display::sz_t midw = width / 2;
  const display::sz_t midh = height / 2;

  layout[0]->pos() = {0, 0};
  layout[1]->pos() = {midw, 0};
  layout[2]->pos() = {width, 0};
  layout[3]->pos() = {width, midh};
  layout[4]->pos() = {width, height};
  layout[5]->pos() = {midw, height};
  layout[6]->pos() = {0, height};
  layout[7]->pos() = {0, midh};
  layout[8]->pos() = {midw, midh};
}

void fill(display::LayoutFree& layout)
{
  using display::pos_t, display::dim_t, display::piv_t;
  using display::PvtX, display::PvtY;
  using display::Window;

  // clang-format off
  layout.append<Window>(renderer, pos_t(), dim_t(10, 5), piv_t(PvtX::Left, PvtY::Top));
  layout.append<Window>(renderer, pos_t(), dim_t(10, 5), piv_t(PvtX::Center, PvtY::Top));
  layout.append<Window>(renderer, pos_t(), dim_t(10, 5), piv_t(PvtX::Right, PvtY::Top));
  layout.append<Window>(renderer, pos_t(), dim_t(10, 5), piv_t(PvtX::Right, PvtY::Center));
  layout.append<Window>(renderer, pos_t(), dim_t(10, 5), piv_t(PvtX::Right, PvtY::Bottom));
  layout.append<Window>(renderer, pos_t(), dim_t(10, 5), piv_t(PvtX::Center, PvtY::Bottom));
  layout.append<Window>(renderer, pos_t(), dim_t(10, 5), piv_t(PvtX::Left, PvtY::Bottom));
  layout.append<Window>(renderer, pos_t(), dim_t(10, 5), piv_t(PvtX::Left, PvtY::Center));
  layout.append<Window>(renderer, pos_t(), dim_t(10, 5), piv_t(PvtX::Center, PvtY::Center));
  // clang-format on
}

}  // namespace

int main()
{
  try {
    using namespace display;  // NOLINT

    auto& display = Display::display();

    auto& layout = display.screen().set_layout<LayoutRigid>(nullptr);
    fill(layout.screen1().set_layout<LayoutFree>(recalculator1));
    fill(layout.screen2().set_layout<LayoutFree>(recalculator2));

    for (display.set_resized(); true;) {
      const auto evnt = display.get_event();
      if (evnt.type() == event::Type::RESIZE) {
        std::cout << alec::erase_display_v<alec::Motion::WHOLE>;
        display.render();
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
