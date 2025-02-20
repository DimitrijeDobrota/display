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
  LayoutRigidBorder(place_t aplc, const layout_t& layout)
      : LayoutRigid<LayoutCustom>(aplc, layout)
  {
    auto [m, n] = get_grid();

    const auto valid = [&](std::size_t xpos, std::size_t ypos)
    { return xpos >= 0 && xpos < n && ypos >= 0 && ypos < m; };

    const auto get = [&](std::size_t xpos, std::size_t ypos) -> std::uint8_t
    { return valid(xpos, ypos) ? layout[xpos][ypos] : 0xFF; };

    for (std::size_t i = 0; i <= n; i++) {
      for (std::size_t j = 0; j <= m; j++) {
        const std::uint8_t ptl = get(i - 1, j - 1);
        const std::uint8_t ptr = get(i - 1, j);
        const std::uint8_t pbl = get(i, j - 1);
        const std::uint8_t pbr = get(i, j);

        uint8_t mask = 0;
        mask |= ((ptl != ptr) ? 1U : 0U) << 0U;  // Top
        mask |= ((ptr != pbr) ? 1U : 0U) << 1U;  // Right
        mask |= ((pbr != pbl) ? 1U : 0U) << 2U;  // Bottom
        mask |= ((pbl != ptl) ? 1U : 0U) << 3U;  // Left

        m_corners.emplace_back(mask, i, j);
      }
    }
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

      set_cursor(pos.y, pos.x + 1);

      for (sz_t j = 1; j < dim.width; j++) {
        std::cout << "─";
      }

      for (sz_t j = pos.y + 1; j < pos.y + dim.height; j++) {
        set_cursor(j, pos.x) << "│";
      }
    }

    for (sz_t i = aypos() + 1; i < aypos() + ahgt(); i++) {
      set_cursor(i, axpos() + awth() - 1) << "│";
    }

    set_cursor(aypos() + ahgt() - 1, axpos() + 1);
    for (sz_t i = 2; i < awth(); i++) {
      std::cout << "─";
    }

    auto [m, n] = get_grid();
    const auto [w, h] = adim();
    const sz_t unw = w / m;
    const sz_t unh = h / n;
    for (const auto [mask, xpos, ypos] : m_corners) {
      const sz_t xloc = xpos != n ? xpos * unw : axpos() + w;
      const sz_t yloc = ypos != m ? ypos * unh : aypos() + h;
      set_cursor(yloc, xloc) << corner_t::lookup[mask];  // NOLINT
    };

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

  struct corner_t
  {
    // clang-format off
    constexpr static const char* lookup[] = { // NOLINT
		"", "", "", "└", "", "│", "┌", "├",
        "", "┘", "─", "┴", "┐", "┤", "┬", "┼"
	};
    // clang-format on

    corner_t(std::uint8_t mask, sz_t ypos, sz_t xpos)  // NOLINT
        : mask(mask)
        , x(xpos)
        , y(ypos)
    {
    }

    std::uint8_t mask;
    sz_t x;
    sz_t y;
  };

  std::vector<corner_t> m_corners;
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
