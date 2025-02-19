#include <iostream>

#include <alec/alec.hpp>

#include "display/display.hpp"
#include "display/layout_rigid.hpp"
#include "display/window_pivot.hpp"

namespace
{

using namespace display;  // NOLINT

class WindowCustom : public WindowPivot
{
public:
  explicit WindowCustom(place_t aplc, piv_t piv, dim_t dim)
      : WindowPivot(aplc, {0, 0}, piv, dim)
  {
  }

  void render() const override
  {
    static int color_red = 0;

    color_red = (color_red + 25) % 256;
    std::cout << alec::background(color_red, 65, 65);

    line_reset();
    for (std::size_t i = 0; i < hgt(); i++) {
      line_empty();
    }

    std::cout << alec::background_v<alec::Color::DEFAULT>;
    std::cout << std::flush;
  }
};

class LayoutCustom : public LayoutRigid<Layout<WindowCustom>>
{
public:
  explicit LayoutCustom(place_t aplc)
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

class LayoutRigidBorder : public LayoutRigid<LayoutCustom>
{
public:
  LayoutRigidBorder(place_t aplc, layout_t layout)
      : LayoutRigid<LayoutCustom>(aplc, std::move(layout))
  {
  }

  template<class... Args>
  LayoutCustom& append(Args&&... args)
  {
    return LayoutMulti<LayoutCustom>::template append<LayoutCustom>(
        place(size()), std::forward<Args>(args)...);
  }

  void resize(place_t aplc) override
  {
    LayoutRigid<LayoutCustom>::resize(aplc);

    for (std::size_t i = 0; i < size(); i++) {
      get(i).resize(place(i));
    }
  }

  void render() const override
  {
    LayoutRigid<LayoutCustom>::render();
    for (std::size_t i = 0; i < size(); i++) {
      const auto [pos, dim] = LayoutRigid<LayoutCustom>::place(i);

      set_cursor(pos.y, pos.x);

      std::cout << "┌";
      for (sz_t j = 1; j < dim.width; j++) {
        std::cout << "─";
      }
      std::cout << "┐";

      for (sz_t j = pos.y + 1; j < pos.y + dim.height; j++) {
        set_cursor(j, pos.x) << "│";
      }
    }

    for (sz_t i = aypos() + 1; i < aypos() + ahgt(); i++) {
      set_cursor(i, axpos() + awth() - 1) << "│";
    }

    set_cursor(aypos() + ahgt() - 1, axpos());
    std::cout << "└";
    for (sz_t i = 2; i < awth(); i++) {
      std::cout << "─";
    }
    std::cout << "┘";

    std::cout << std::flush;
  }

private:
  place_t place(std::size_t idx)
  {
    const auto [pos, dim] = LayoutRigid<LayoutCustom>::place(idx);
    dim_t sub = {1, 1};

    if (get_record(idx).addw) {
      sub.width += 1;
    }

    if (get_record(idx).addh) {
      sub.height += 1;
    }

    return {pos + pos_t(1, 1), dim - sub};
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
        {0, 4, 2},
        {3, 4, 2},
    };
    // clang-format on

    auto& layout = display.layout().set_child<LayoutRigidBorder>(split);
    layout.append();
    layout.append();
    layout.append();
    layout.append();
    layout.append();

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
