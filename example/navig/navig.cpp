#include <iostream>
#include <stack>
#include <string>

#include "display/display.hpp"
#include "display/layout.hpp"
#include "display/window_pivot.hpp"
#include "menu.hpp"

namespace
{

using namespace display::literals;  // NOLINT(*namespace*)

bool is_finished = false;  // NOLINT

using display::WindowPivot;

class WindowCustom : public WindowPivot
{
public:
  WindowCustom(
      display::plc_t aplc, display::piv_t piv, const example::menu_t& menu
  )
      // NOLINTNEXTLINE(*magic*)
      : WindowPivot(aplc, {4_w, 2_h, 5_w, 4_h}, piv, calc_dim(menu))
      , m_menu(menu)
  {
  }

  void render() const override
  {
    std::cout << alec::background_v<alec::color::blue>;
    line_reset();

    line_center(m_menu.title);
    line_empty();
    for (std::size_t i = 0; i < m_menu.items.size(); i++) {
      if (m_selected == i) {
        std::cout << alec::foreground_v<alec::color::green>;
      }

      line_right(m_menu.items[i].prompt);

      if (m_selected == i) {
        std::cout << alec::foreground_v<alec::color::def>;
      }
    }

    WindowPivot::render();
    WindowPivot::render_border();

    std::cout << alec::background_v<alec::color::def>;
    std::cout << std::flush;
  }

  void input(display::event& evnt) override
  {
    if (evnt.type() != display::event::type::key) {
      return;
    }

    if (evnt.key() == 'j') {
      if (m_selected + 1U < m_menu.items.size()) {
        m_selected++;
      }
      evnt.type() = display::event::type::none;
      render();
      return;
    }

    if (evnt.key() == 'k') {
      if (m_selected > 0) {
        m_selected--;
      }
      evnt.type() = display::event::type::none;
      render();
      return;
    }

    if (evnt.key() == 'l') {
      m_menu.items[m_selected].callback(m_selected);
      evnt.type() = display::event::type::none;
      return;
    }

    if (evnt.key() == 'h') {
      m_menu.callback(0);
      evnt.type() = display::event::type::none;
      return;
    }
  }

private:
  static display::dim_t calc_dim(const example::menu_t& menu)
  {
    using wth_t = display::wth_t;
    using hgt_t = display::hgt_t;

    wth_t width {menu.title.size()};
    for (const auto& item : menu.items) {
      const wth_t lwidth {item.prompt.size()};
      width = std::max(width, lwidth);
    }

    const hgt_t height {menu.items.size() + 2};
    return {width, height};
  }

  example::menu_t m_menu;
  uint8_t m_selected = 0;
};

}  // namespace

namespace example
{

int operation1(std::size_t /* unused */)  // NOLINT
{
  std::cout << alec::cursor_position(1, 1) << "operation 1";
  std::cout << alec::cursor_position(2, 1)
            << alec::erase_line_v<alec::motion::whole>
            << "Some operation is done";
  std::cout << std::flush;
  return 1;
}

int operation2(std::size_t /* unused */)  // NOLINT
{
  std::cout << alec::cursor_position(1, 1) << "operation 2";
  std::cout << alec::cursor_position(2, 1)
            << alec::erase_line_v<alec::motion::whole>
            << "Some other operation is done";
  std::cout << std::flush;
  return 1;
}

int operation3(std::size_t /* unused */)  // NOLINT
{
  std::cout << alec::cursor_position(1, 1) << "operation 3";
  std::cout << alec::cursor_position(2, 1)
            << alec::erase_line_v<alec::motion::whole>
            << "Yet another operation is done";
  std::cout << std::flush;
  return 1;
}

int finish(std::size_t /* unused */)  // NOLINT
{
  std::cout << alec::cursor_position(1, 1)
            << alec::erase_line_v<alec::motion::whole>;
  std::cout << "finishing...";
  std::cout << std::flush;
  is_finished = true;
  return 0;
}

int menu_t::visit(const menu_t& menu)
{
  using display::Display, display::Layout;
  using display::piv_t;

  auto& layout = Display::display().layout();

  static std::stack<const menu_t*> stk;

  if (!stk.empty() && stk.top()->title == menu.title) {
    stk.pop();
    if (stk.empty()) {
      finish(0);
      return 0;
    }
  } else {
    stk.push(&menu);
  }

  layout.set_child<WindowCustom>(
      piv_t(piv_t::x::right, piv_t::y::bottom), *stk.top()
  );
  layout.render();

  return 0;
}

}  // namespace example

int main()
{
  try {
    using namespace display;  // NOLINT

    auto& display = Display::display();
    example::menu_main(0);

    while (!is_finished) {
      auto evnt = display.get_event();
      if (evnt.type() == event::type::resize) {
        std::cout << alec::erase_display_v<alec::motion::whole>;
        display.render();
        continue;
      }

      if (evnt.type() == event::type::key) {
        if (evnt.key() == 'q') {
          break;
        }
        display.input(evnt);
      }
    }
  } catch (std::exception& err) {
    std::cout << err.what() << '\n' << std::flush;
  }

  return 0;
}
