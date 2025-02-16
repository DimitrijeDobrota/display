#include <iostream>
#include <string>

#include <alec/alec.hpp>

#include "display/display.hpp"
#include "display/layout_rigid.hpp"
#include "display/window_pivot.hpp"

namespace
{

class WindowCustom : public display::WindowPivot
{
public:
  WindowCustom(display::aplace_t aplc, display::dim_t dim, display::piv_t piv)
      : WindowPivot(aplc, dim, piv)
  {
  }

  void render() const override
  {
    static int color_red = 0;
    color_red = (color_red + 25) % 256;

    const auto [apos, adim] = place();
    const auto [x, y] = apos;
    const auto [w, h] = adim;

    std::cout << alec::background(color_red, 65, 65);

    for (auto ypos = y; ypos < y + h; ypos++) {
      std::cout << alec::cursor_position(ypos + 1, x + 1);
      std::cout << std::string(w, ' ');
    }

    std::cout << alec::background_v<alec::Color::DEFAULT>;
    std::cout << std::flush;
  }
};

class LayoutCustom : public display::LayoutMulti<WindowCustom>
{
public:
  explicit LayoutCustom(display::aplace_t aplc)
      : LayoutMulti(aplc)
  {
    using display::dim_t, display::piv_t;
    using display::PvtX, display::PvtY;

    append(dim_t(12, 4), piv_t(PvtX::Left, PvtY::Top));
    append(dim_t(12, 4), piv_t(PvtX::Center, PvtY::Top));
    append(dim_t(12, 4), piv_t(PvtX::Right, PvtY::Top));
    append(dim_t(12, 4), piv_t(PvtX::Right, PvtY::Center));
    append(dim_t(12, 4), piv_t(PvtX::Right, PvtY::Bottom));
    append(dim_t(12, 4), piv_t(PvtX::Center, PvtY::Bottom));
    append(dim_t(12, 4), piv_t(PvtX::Left, PvtY::Bottom));
    append(dim_t(12, 4), piv_t(PvtX::Left, PvtY::Center));
    append(dim_t(12, 4), piv_t(PvtX::Center, PvtY::Center));

    recalc();
  }

  void resize(display::aplace_t aplc) override
  {
    LayoutMulti::resize(aplc);
    recalc();
  }

private:
  void recalc()
  {
    const auto [width, height] = adim();
    const display::sz_t midw = width / 2;
    const display::sz_t midh = height / 2;

    get(0).set_pos({0, 0});
    get(1).set_pos({midw, 0});
    get(2).set_pos({width, 0});
    get(3).set_pos({width, midh});
    get(4).set_pos({width, height});
    get(5).set_pos({midw, height});
    get(6).set_pos({0, height});
    get(7).set_pos({0, midh});
    get(8).set_pos({midw, midh});
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
