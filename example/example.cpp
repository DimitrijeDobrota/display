#include <iostream>
#include <string>

#include "alec/alec.hpp"
#include "display/display.hpp"
#include "display/layout_free.hpp"
#include "display/layout_rigid.hpp"
#include "display/window_pivot.hpp"

namespace
{

class WindowCustom : public display::WindowPivot
{
public:
  WindowCustom(display::dim_t dim, display::piv_t piv)
      : WindowPivot(display::pos_t(), dim, piv)
  {
  }

  void render(display::aplace_t aplc) const override
  {
    static int color_red = 0;
    color_red = (color_red + 25) % 256;

    const auto [start, end] = aplc;

    std::cout << alec::background(color_red, 65, 65);
    for (auto ypos = start.y; ypos < end.y; ypos++) {
      std::cout << alec::cursor_position(ypos + 1, start.x + 1);
      std::cout << std::string(end.x - start.x, ' ');
    }
    std::cout << alec::background_v<alec::Color::DEFAULT>;
    std::cout << std::flush;
  }
};

class LayoutCustom : public display::LayoutFree
{
public:
  LayoutCustom()
  {
    using display::dim_t, display::piv_t;
    using display::PvtX, display::PvtY;

    append<WindowCustom>(dim_t(12, 4), piv_t(PvtX::Left, PvtY::Top));
    append<WindowCustom>(dim_t(12, 4), piv_t(PvtX::Center, PvtY::Top));
    append<WindowCustom>(dim_t(12, 4), piv_t(PvtX::Right, PvtY::Top));
    append<WindowCustom>(dim_t(12, 4), piv_t(PvtX::Right, PvtY::Center));
    append<WindowCustom>(dim_t(12, 4), piv_t(PvtX::Right, PvtY::Bottom));
    append<WindowCustom>(dim_t(12, 4), piv_t(PvtX::Center, PvtY::Bottom));
    append<WindowCustom>(dim_t(12, 4), piv_t(PvtX::Left, PvtY::Bottom));
    append<WindowCustom>(dim_t(12, 4), piv_t(PvtX::Left, PvtY::Center));
    append<WindowCustom>(dim_t(12, 4), piv_t(PvtX::Center, PvtY::Center));
  }

  void resize(display::apos_t apos, display::dim_t dim) override
  {
    LayoutFree::resize(apos, dim);

    const auto [width, height] = dim;
    const display::sz_t midw = width / 2;
    const display::sz_t midh = height / 2;

    get<WindowCustom>(0).pos() = {0, 0};
    get<WindowCustom>(1).pos() = {midw, 0};
    get<WindowCustom>(2).pos() = {width, 0};
    get<WindowCustom>(3).pos() = {width, midh};
    get<WindowCustom>(4).pos() = {width, height};
    get<WindowCustom>(5).pos() = {midw, height};
    get<WindowCustom>(6).pos() = {0, height};
    get<WindowCustom>(7).pos() = {0, midh};
    get<WindowCustom>(8).pos() = {midw, midh};
  }
};

}  // namespace

int main()
{
  try {
    using namespace display;  // NOLINT

    auto& display = Display::display();

    // clang-format off
    const LayoutRigid::layout_t split = {
        {1, 1, 2},
        {0, 3, 2},
        {4, 3, 2},
    };
    // clang-format on

    auto& layout = display.screen().set_layout<LayoutRigid>(split);
    layout[0].set_layout<LayoutCustom>();
    layout[1].set_layout<LayoutCustom>();
    layout[2].set_layout<LayoutCustom>();
    layout[3].set_layout<LayoutCustom>();
    layout[4].set_layout<LayoutCustom>();

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
