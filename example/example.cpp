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
  explicit WindowCustom(plc_t aplc, piv_t piv, dim_t dim)
      : WindowPivot(aplc, pad_t(0, 0), piv, dim)
  {
  }

  void render() const override
  {
    static int color_red = 0;

    color_red = (color_red + 25) % 256;
    std::cout << alec::background(color_red, 65, 65);

    line_reset();
    for (auto i = hgt_t(0); i < hgt(); i++) {
      line_empty();
    }

    std::cout << alec::background_v<alec::Color::DEFAULT>;
    std::cout << std::flush;
  }
};

class LayoutCustom : public LayoutRigid<Layout<WindowCustom>>
{
public:
  explicit LayoutCustom(plc_t aplc)
      : LayoutRigid(aplc, {{0, 1, 2}, {7, 8, 3}, {6, 5, 4}})
  {
    emplace().emplace_child(piv_t(PvtX::Left, PvtY::Top), dim_t(12, 4));
    emplace().emplace_child(piv_t(PvtX::Center, PvtY::Top), dim_t(12, 4));
    emplace().emplace_child(piv_t(PvtX::Right, PvtY::Top), dim_t(12, 4));
    emplace().emplace_child(piv_t(PvtX::Right, PvtY::Center), dim_t(12, 4));
    emplace().emplace_child(piv_t(PvtX::Right, PvtY::Bottom), dim_t(12, 4));
    emplace().emplace_child(piv_t(PvtX::Center, PvtY::Bottom), dim_t(12, 4));
    emplace().emplace_child(piv_t(PvtX::Left, PvtY::Bottom), dim_t(12, 4));
    emplace().emplace_child(piv_t(PvtX::Left, PvtY::Center), dim_t(12, 4));
    emplace().emplace_child(piv_t(PvtX::Center, PvtY::Center), dim_t(12, 4));
  }
};

class LayoutRigidBorder : public LayoutRigid<LayoutCustom>
{
public:
  LayoutRigidBorder(plc_t aplc, const layout_t& layout)
      : LayoutRigid<LayoutCustom>(aplc, layout)
  {
    const auto [m, n] = get_grid();

    const auto valid = [&](std::size_t xpos, std::size_t ypos)
    { return xpos < n && ypos < m; };

    const auto get = [&](std::size_t xpos, std::size_t ypos) -> std::uint8_t
    { return valid(xpos, ypos) ? layout[xpos][ypos] : 0xFF; };

    for (std::size_t i = 0; i <= n; i++) {
      for (std::size_t j = 0; j <= m; j++) {
        const std::uint8_t ptl = get(i - 1, j - 1);
        const std::uint8_t ptr = get(i - 1, j);
        const std::uint8_t pbl = get(i, j - 1);
        const std::uint8_t pbr = get(i, j);

        uint8_t mask = 0;
        mask |= static_cast<uint8_t>(((ptl != ptr) ? 1U : 0U) << 0U);  // Top
        mask |= static_cast<uint8_t>(((ptr != pbr) ? 1U : 0U) << 1U);  // Right
        mask |= static_cast<uint8_t>(((pbr != pbl) ? 1U : 0U) << 2U);  // Bottom
        mask |= static_cast<uint8_t>(((pbl != ptl) ? 1U : 0U) << 3U);  // Left

        m_corners.emplace_back(mask, wth_t(j), hgt_t(i));
      }
    }
  }

  template<class... Args>
  LayoutCustom& emplace(Args&&... args)
  {
    return LayoutMulti<LayoutCustom>::template emplace<LayoutCustom>(
        place(size()), std::forward<Args>(args)...);
  }

  void resize(plc_t aplc) override
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

      // Top of each element
      set_cursor(pos.x + 1, pos.y);
      for (auto j = wth_t(1); j < dim.width; j++) {
        std::cout << "─";
      }

      // Left of each element
      for (auto j = pos.y + 1; j < pos.y + dim.height; j++) {
        set_cursor(pos.x, j) << "│";
      }
    }

    // Right of the layout
    for (auto j = aypos() + 1; j < aypos() + ahgt(); j++) {
      set_cursor(axpos() + awth() - 1, j) << "│";
    }

    // Bottom of the layout
    set_cursor(axpos() + 1, aypos() + ahgt() - 1);
    for (auto i = wth_t(2); i < awth(); i++) {
      std::cout << "─";
    }

    const auto [m, n] = get_grid();
    const auto [w, h] = adim();
    const auto unw = w / m;
    const auto unh = h / n;
    for (const auto [mask, wth, hgt] : m_corners) {
      const auto xloc = wth != m ? xpos_t((wth * unw).value()) : axpos() + w;
      const auto yloc = hgt != n ? ypos_t((hgt * unh).value()) : aypos() + h;
      set_cursor(xloc, yloc) << corner_t::lookup[mask];  // NOLINT
    };

    std::cout << std::flush;
  }

private:
  plc_t place(std::size_t idx)
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

    corner_t(std::uint8_t maskv, wth_t wigth, hgt_t height)  // NOLINT
        : mask(maskv)
        , wth(wigth)
        , hgt(height)
    {
    }

    std::uint8_t mask;
    wth_t wth;
    hgt_t hgt;
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

    auto& layout = display.layout().emplace_child<LayoutRigidBorder>(split);
    layout.emplace();
    layout.emplace();
    layout.emplace();
    layout.emplace();
    layout.emplace();

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
