#include <iostream>

#include <alec/alec.hpp>

#include "display/display.hpp"
#include "display/layout_rigid.hpp"
#include "display/window_pivot.hpp"

namespace
{

using namespace based::literals;  // NOLINT(*namespace*)

using namespace display;  // NOLINT(*namespace*)
using namespace based::literals;  // NOLINT(*namespace*)

class WindowCustom : public WindowPivot
{
public:
  explicit WindowCustom(plc_t aplc, piv_t piv, dim_t dim)
      : WindowPivot(aplc, pad_t(0_w, 0_h), piv, dim)
  {
  }

  void render() const override
  {
    static constexpr based::u8 inc = 0_u8;
    static constexpr based::u8 mod = 256_u8;
    static constexpr based::u8 color_green = 0_u8;
    static constexpr based::u8 color_blue = 0_u8;
    static based::u8 color_red = 0_u8;

    color_red = (color_red + inc) % mod;
    std::cout << alec::background(
        color_red.value, color_green.value, color_blue.value
    );

    line_reset();
    for (auto i = 0_h; i < hgt(); ++i) {
      line_empty();
    }

    std::cout << alec::background_v<alec::color::def>;
    std::cout << std::flush;
  }
};

class LayoutCustom : public LayoutRigid<Layout<WindowCustom>>
{
public:
  explicit LayoutCustom(plc_t aplc)
      // NOLINTNEXTLINE(*magic*)
      : LayoutRigid(aplc, {{0, 1, 2}, {7, 8, 3}, {6, 5, 4}})
  {
    using x = piv_t::x;
    using y = piv_t::y;

    const dim_t dim(12_w, 4_h);

    append().set_child(piv_t(x::left, y::top), dim);
    append().set_child(piv_t(x::center, y::top), dim);
    append().set_child(piv_t(x::right, y::top), dim);
    append().set_child(piv_t(x::right, y::center), dim);
    append().set_child(piv_t(x::right, y::bottom), dim);
    append().set_child(piv_t(x::center, y::bottom), dim);
    append().set_child(piv_t(x::left, y::bottom), dim);
    append().set_child(piv_t(x::left, y::center), dim);
    append().set_child(piv_t(x::center, y::center), dim);
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
    {
      return xpos < n.value && ypos < m.value;
    };

    const auto get = [&](std::size_t xpos, std::size_t ypos) -> based::bu8
    {
      static constexpr auto not_found = 0xFF;
      return valid(xpos, ypos) ? layout[xpos][ypos] : not_found;
    };

    for (std::size_t i = 0; i <= n.value; i++) {
      for (std::size_t j = 0; j <= m.value; j++) {
        const based::bu8 ptl = get(i - 1, j - 1);
        const based::bu8 ptr = get(i - 1, j);
        const based::bu8 pbl = get(i, j - 1);
        const based::bu8 pbr = get(i, j);

        based::bu8 mask = 0;
        mask |= ((ptl != ptr) ? 1U : 0U);  // Top
        mask |= ((ptr != pbr) ? 1U : 0U) << 1U;  // Right
        mask |= ((pbr != pbl) ? 1U : 0U) << 2U;  // Bottom
        mask |= ((pbl != ptl) ? 1U : 0U) << 3U;  // Left

        m_corners.emplace_back(mask, wth_t(j), hgt_t(i));
      }
    }
  }

  template<class... Args>
  LayoutCustom& append(Args&&... args)
  {
    return LayoutMulti<LayoutCustom>::template append<LayoutCustom>(
        place(size()), std::forward<Args>(args)...
    );
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
      set_cursor(pos.x + 1_x, pos.y);
      for (auto j = 1_w; j < dim.width; ++j) {
        std::cout << "─";
      }

      // Left of each element
      for (auto j = pos.y + 1_y; j < pos.y + dim.height; ++j) {
        set_cursor(pos.x, j) << "│";
      }
    }

    // Right of the layout
    for (auto j = aypos() + 1_y; j < aypos() + ahgt(); ++j) {
      set_cursor(axpos() + awth() - 1_w, j) << "│";
    }

    // Bottom of the layout
    set_cursor(axpos() + 1_x, aypos() + ahgt() - 1_h);
    for (auto i = 2_w; i < awth(); ++i) {
      std::cout << "─";
    }

    const auto [m, n] = get_grid();
    const auto [w, h] = adim();
    const auto unw = w / m;
    const auto unh = h / n;
    for (const auto [mask, wth, hgt] : m_corners) {
      const auto xloc = wth != m ? xpos_t((wth * unw).value) : axpos() + w;
      const auto yloc = hgt != n ? ypos_t((hgt * unh).value) : aypos() + h;
      set_cursor(xloc, yloc) << corner_t::lookup[mask];  // NOLINT
    };

    std::cout << std::flush;
  }

private:
  plc_t place(std::size_t idx)
  {
    const auto [pos, dim] = LayoutRigid<LayoutCustom>::place(idx);
    dim_t sub = {1_w, 1_h};

    if (get_record(idx).addw) {
      sub.width += 1_w;
    }

    if (get_record(idx).addh) {
      sub.height += 1_h;
    }

    return {pos + pos_t(1_x, 1_y), dim - sub};
  }

  struct corner_t
  {
    // clang-format off
    constexpr static const char* lookup[] = { // NOLINT
		"", "", "", "└", "", "│", "┌", "├",
        "", "┘", "─", "┴", "┐", "┤", "┬", "┼"
	};
    // clang-format on

    corner_t(based::bu8 msk, wth_t wth, hgt_t hgt)  // NOLINT
        : mask(msk)
        , wigth(wth)
        , height(hgt)
    {
    }

    based::bu8 mask;
    wth_t wigth;
    hgt_t height;
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
      if (evnt.type() == event::type::resize) {
        std::cout << alec::erase_display_v<alec::motion::whole>;
        display.render();
        continue;
      }

      if (evnt.type() == event::type::key && evnt.key() == 'q') {
        break;
      }
    }
  } catch (std::exception& err) {
    std::cout << err.what() << '\n' << std::flush;
  }

  return 0;
}
