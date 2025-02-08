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

enum class PvtX : std::uint8_t
{
  Left,
  Center,
  Right
};

enum class PvtY : std::uint8_t
{
  Top,
  Center,
  Bottom
};

struct piv_t
{
  piv_t(PvtX pvtx = PvtX::Left, PvtY pvty = PvtY::Top)  // NOLINT
      : x(pvtx)
      , y(pvty)
  {
  }

  PvtX x;
  PvtY y;
};

class Window
{
public:
  Window(pos_t pos, dim_t dim, piv_t piv = {})
      : m_pos(pos)
      , m_dim(dim)
      , m_piv(piv)
  {
  }

  const auto& pos() const { return m_pos; }
  auto& pos() { return m_pos; }

  const auto& dim() const { return m_dim; }
  auto& dim() { return m_dim; }

  const auto& piv() const { return m_piv; }
  auto& piv() { return m_piv; }

private:
  pos_t m_pos;
  dim_t m_dim;
  piv_t m_piv;
};

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
