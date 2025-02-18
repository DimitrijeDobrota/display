#include <iostream>
#include <stack>
#include <string>

#include <stamen/stamen.hpp>

#include "display/display.hpp"
#include "display/layout.hpp"
#include "display/window_pivot.hpp"
#include "menu.hpp"

namespace
{

bool is_finished = false;  // NOLINT

class WindowCustom : public display::WindowPivot
{
public:
  WindowCustom(display::aplace_t aplc,
               display::piv_t piv,
               const example::menu_t& menu)
      : WindowPivot(aplc, piv, calc_dim(menu))
      , m_menu(menu)
  {
  }

  void render() const override
  {
    std::cout << alec::background_v<alec::Color::BLUE>;

    line_empty(/* reset = */ true);
    line_center(m_menu.title);
    line_empty();
    for (std::size_t i = 0; i < m_menu.items.size(); i++) {
      if (m_selected == i) {
        std::cout << alec::foreground_v<alec::Color::GREEN>;
      }

      line_right(m_menu.items[i].prompt);

      if (m_selected == i) {
        std::cout << alec::foreground_v<alec::Color::DEFAULT>;
      }
    }
    line_empty();

    std::cout << alec::background_v<alec::Color::DEFAULT>;
    std::cout << std::flush;
  }

  void input(display::event& evnt) override
  {
    if (evnt.type() != display::event::Type::KEY) {
      return;
    }

    if (evnt.key() == 'j') {
      if (m_selected + 1 < m_menu.items.size()) {
        m_selected++;
      }
      evnt.type() = display::event::Type::NONE;
      render();
      return;
    }

    if (evnt.key() == 'k') {
      if (m_selected > 0) {
        m_selected--;
      }
      evnt.type() = display::event::Type::NONE;
      render();
      return;
    }

    if (evnt.key() == 'l') {
      m_menu.items[m_selected].callback(m_selected);
      evnt.type() = display::event::Type::NONE;
      return;
    }

    if (evnt.key() == 'h') {
      m_menu.callback(0);
      evnt.type() = display::event::Type::NONE;
      return;
    }
  }

private:
  static display::dim_t calc_dim(const example::menu_t& menu)
  {
    std::size_t width = menu.title.size();
    for (const auto& item : menu.items) {
      width = std::max(width, item.prompt.size());
    }

    return {static_cast<display::sz_t>(width + 2),
            static_cast<display::sz_t>(menu.items.size() + 4)};
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
            << alec::erase_line_v<alec::Motion::WHOLE>
            << "Some operation is done";
  std::cout << std::flush;
  return 1;
}

int operation2(std::size_t /* unused */)  // NOLINT
{
  std::cout << alec::cursor_position(1, 1) << "operation 2";
  std::cout << alec::cursor_position(2, 1)
            << alec::erase_line_v<alec::Motion::WHOLE>
            << "Some other operation is done";
  std::cout << std::flush;
  return 1;
}

int operation3(std::size_t /* unused */)  // NOLINT
{
  std::cout << alec::cursor_position(1, 1) << "operation 3";
  std::cout << alec::cursor_position(2, 1)
            << alec::erase_line_v<alec::Motion::WHOLE>
            << "Yet another operation is done";
  std::cout << std::flush;
  return 1;
}

int finish(std::size_t /* unused */)  // NOLINT
{
  std::cout << alec::cursor_position(1, 1)
            << alec::erase_line_v<alec::Motion::WHOLE>;
  std::cout << "finishing...";
  std::cout << std::flush;
  is_finished = true;
  return 0;
}

int menu_t::visit(const menu_t& menu)
{
  using display::Display, display::Layout;
  using display::PvtX, display::PvtY, display::piv_t;

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

  layout.set_child<WindowCustom>(piv_t(PvtX::Center, PvtY::Center), *stk.top());
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
      if (evnt.type() == event::Type::RESIZE) {
        std::cout << alec::erase_display_v<alec::Motion::WHOLE>;
        display.render();
        continue;
      }

      if (evnt.type() == event::Type::KEY) {
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
