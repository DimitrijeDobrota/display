#pragma once

#include <vector>

#include "display/window.hpp"

namespace display
{

class LayoutFree
{
public:
  using render_f = int(Window&);

  const auto& operator[](std::size_t idx) const { return m_windows[idx]; }
  auto& operator[](std::size_t idx) { return m_windows[idx]; }

  void append(Window window);

  int render(render_f renderer);

private:
  std::vector<Window> m_windows;
  bool m_is_sorted = true;
};

class LayoutRigid
{
public:
private:
};

}  // namespace display
