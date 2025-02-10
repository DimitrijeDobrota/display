#pragma once

#include <vector>

#include "display/window.hpp"

namespace display
{

class Screen;

class LayoutFree
{
public:
  using render_f = int(Window&);
  using recalc_f = void(LayoutFree&);

  LayoutFree(recalc_f f_recalc)  // NOLINT
      : m_recalc(f_recalc)
  {
  }

  dim_t dim() const;

  Screen* get_screen() { return m_screen; }
  void set_screen(Screen* screen) { m_screen = screen; }

  const auto& operator[](std::size_t idx) const { return m_windows[idx]; }
  auto& operator[](std::size_t idx) { return m_windows[idx]; }
  void append(Window window);

  void recalc();
  int render(render_f renderer);

private:
  std::vector<Window> m_windows;
  bool m_is_sorted = true;
  recalc_f* m_recalc;

  Screen* m_screen = nullptr;
};

class LayoutRigid
{
public:
private:
};

}  // namespace display
