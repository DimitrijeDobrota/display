#include <iostream>
#include <string>

#include "alec/alec.hpp"
#include "display/display.hpp"
#include "display/layout_free.hpp"
#include "display/layout_rigid.hpp"
#include "display/window_pivot.hpp"

namespace
{

void renderer(const display::WindowPivot& win, display::place_t plc)
{
  using display::place_t;

  static int color_red = 0;
  color_red = (color_red + 25) % 256;

  const auto [start, end] = plc;

  std::cout << alec::background(color_red, 65, 65);
  for (auto ypos = start.y; ypos < end.y; ypos++) {
    std::cout << alec::cursor_position(ypos + 1, start.x + 1);
    std::cout << std::string(end.x - start.x, ' ');
  }
  std::cout << alec::background_v<alec::Color::DEFAULT>;
  std::cout << std::flush;

  (void)win;
}

void fill(display::LayoutFree& layout)
{
  using display::pos_t, display::dim_t, display::piv_t;
  using display::PvtX, display::PvtY;
  using display::WindowPivot;

  // clang-format off
  layout.append<WindowPivot>(renderer, pos_t(), dim_t(12, 4), piv_t(PvtX::Left, PvtY::Top));
  layout.append<WindowPivot>(renderer, pos_t(), dim_t(12, 4), piv_t(PvtX::Center, PvtY::Top));
  layout.append<WindowPivot>(renderer, pos_t(), dim_t(12, 4), piv_t(PvtX::Right, PvtY::Top));
  layout.append<WindowPivot>(renderer, pos_t(), dim_t(12, 4), piv_t(PvtX::Right, PvtY::Center));
  layout.append<WindowPivot>(renderer, pos_t(), dim_t(12, 4), piv_t(PvtX::Right, PvtY::Bottom));
  layout.append<WindowPivot>(renderer, pos_t(), dim_t(12, 4), piv_t(PvtX::Center, PvtY::Bottom));
  layout.append<WindowPivot>(renderer, pos_t(), dim_t(12, 4), piv_t(PvtX::Left, PvtY::Bottom));
  layout.append<WindowPivot>(renderer, pos_t(), dim_t(12, 4), piv_t(PvtX::Left, PvtY::Center));
  layout.append<WindowPivot>(renderer, pos_t(), dim_t(12, 4), piv_t(PvtX::Center, PvtY::Center));
  // clang-format on
}

}  // namespace

int main()
{
  try {
    using namespace std::placeholders;  // NOLINT
    using namespace display;  // NOLINT

    const auto recalc = [](LayoutFree& layout)
    {
      const auto [width, height] = layout.dim();
      const display::sz_t midw = width / 2;
      const display::sz_t midh = height / 2;

      layout.get<WindowPivot>(0).pos() = {0, 0};
      layout.get<WindowPivot>(1).pos() = {midw, 0};
      layout.get<WindowPivot>(2).pos() = {width, 0};
      layout.get<WindowPivot>(3).pos() = {width, midh};
      layout.get<WindowPivot>(4).pos() = {width, height};
      layout.get<WindowPivot>(5).pos() = {midw, height};
      layout.get<WindowPivot>(6).pos() = {0, height};
      layout.get<WindowPivot>(7).pos() = {0, midh};
      layout.get<WindowPivot>(8).pos() = {midw, midh};
    };

    auto& display = Display::display();

    // clang-format off
    const LayoutRigid::layout_t split = {
        {1, 1, 2},
        {0, 3, 2},
        {4, 3, 2},
    };
    // clang-format on

    auto& layout = display.screen().set_layout<LayoutRigid>(split, nullptr);
    fill(layout[0].set_layout<LayoutFree>(recalc));
    fill(layout[1].set_layout<LayoutFree>(recalc));
    fill(layout[2].set_layout<LayoutFree>(recalc));
    fill(layout[3].set_layout<LayoutFree>(recalc));
    fill(layout[4].set_layout<LayoutFree>(recalc));

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
