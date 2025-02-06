#pragma once

#include <cstdint>
#include <utility>
#include <vector>

namespace display
{

class Window
{
public:
  Window(std::uint16_t xpos,
         std::uint16_t ypos,
         std::uint16_t width,
         std::uint16_t height)
      : m_xpos(xpos)
      , m_ypos(ypos)
      , m_width(width)
      , m_height(height)
  {
  }

  auto xpos() const { return m_xpos; }
  auto ypos() const { return m_ypos; }
  auto width() const { return m_width; }
  auto height() const { return m_height; }

private:
  std::uint16_t m_xpos;
  std::uint16_t m_ypos;
  std::uint16_t m_width;
  std::uint16_t m_height;
};

class LayoutFree
{
public:
  using render_f = int(Window&);

  void add_window(Window window, int zidx = -1);
  int render(render_f renderer);

private:
  std::vector<std::pair<int, Window>> m_windows;
  bool m_is_sorted = true;
};

class LayoutRigid
{
public:
private:
};

}  // namespace display
