#pragma once

#include <cstdint>
#include <utility>
#include <vector>

namespace display
{

using sz_t = std::uint16_t;

struct dim_t
{
  dim_t(sz_t wdth = 0, sz_t hght = 0)  // NOLINT
      : width(wdth)
      , height(hght)
  {
  }

  sz_t width;
  sz_t height;
};

struct pos_t
{
  pos_t(sz_t xpos = 0, sz_t ypos = 0, sz_t zpos = 0)  // NOLINT
      : x(xpos)
      , y(ypos)
      , z(zpos)
  {
  }

  sz_t x;
  sz_t y;
  sz_t z;
};

class Window
{
public:
  Window(pos_t pos, dim_t dim)
      : m_pos(pos)
      , m_dim(dim)
  {
  }

  const auto& pos() const { return m_pos; }
  auto& pos() { return m_pos; }

  const auto& dim() const { return m_dim; }
  auto& dim() { return m_dim; }

private:
  pos_t m_pos;
  dim_t m_dim;
};

class LayoutFree
{
public:
  using render_f = int(Window&);

  auto operator[](std::size_t idx) const { return m_windows[idx]; }
  auto operator[](std::size_t idx) { return m_windows[idx]; }

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
