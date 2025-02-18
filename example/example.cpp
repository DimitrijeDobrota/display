#include <iostream>

#include <alec/alec.hpp>

#include "display/display.hpp"
#include "display/layout_rigid.hpp"
#include "display/window_pivot.hpp"

namespace
{

using display::PvtX, display::PvtY;
using display::sz_t, display::dim_t, display::piv_t, display::place_t;

class WindowCustom : public display::WindowPivot
{
public:
  explicit WindowCustom(place_t aplc, piv_t piv, dim_t dim)
      : WindowPivot(aplc, piv, dim)
  {
  }

  void render() const override
  {
    static int color_red = 0;

    color_red = (color_red + 25) % 256;
    std::cout << alec::background(color_red, 65, 65);

    line_empty(/* reset = */ true);
    for (std::size_t i = 1; i < ahgt(); i++) {
      line_empty();
    }

    std::cout << alec::background_v<alec::Color::DEFAULT>;
    std::cout << std::flush;
  }
};

class LayoutCustom : public display::LayoutRigid<display::Layout<WindowCustom>>
{
public:
  explicit LayoutCustom(display::place_t aplc)
      : LayoutRigid(aplc, {{0, 1, 2}, {7, 8, 3}, {6, 5, 4}})
  {
    append().set_child(piv_t(PvtX::Left, PvtY::Top), dim_t(12, 4));
    append().set_child(piv_t(PvtX::Center, PvtY::Top), dim_t(12, 4));
    append().set_child(piv_t(PvtX::Right, PvtY::Top), dim_t(12, 4));
    append().set_child(piv_t(PvtX::Right, PvtY::Center), dim_t(12, 4));
    append().set_child(piv_t(PvtX::Right, PvtY::Bottom), dim_t(12, 4));
    append().set_child(piv_t(PvtX::Center, PvtY::Bottom), dim_t(12, 4));
    append().set_child(piv_t(PvtX::Left, PvtY::Bottom), dim_t(12, 4));
    append().set_child(piv_t(PvtX::Left, PvtY::Center), dim_t(12, 4));
    append().set_child(piv_t(PvtX::Center, PvtY::Center), dim_t(12, 4));
  }
};

}  // namespace

int main()
{
  try {
    using namespace display;  // NOLINT

    auto& display = Display::display();

    // clang-format off
    const LayoutRigid<>::layout_t split = {
        {1, 1, 2},
        {0, 3, 2},
        {4, 3, 2},
    };
    // clang-format on

    auto& layout = display.layout().set_child<LayoutRigid<>>(split);
    layout.append<LayoutCustom>();
    layout.append<LayoutCustom>();
    layout.append<LayoutCustom>();
    layout.append<LayoutCustom>();
    layout.append<LayoutCustom>();

    display.render();
    while (true) {
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
