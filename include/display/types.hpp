#pragma once

#include <cstdint>
#include <utility>

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

  dim_t(std::pair<sz_t, sz_t> pair)  // NOLINT
      : width(std::get<0>(pair))
      , height(std::get<1>(pair))
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

  pos_t operator+(pos_t rhs) const
  {
    return {static_cast<std::uint16_t>(x + rhs.x),
            static_cast<std::uint16_t>(y + rhs.y),
            static_cast<std::uint16_t>(z + rhs.z)};
  }

  sz_t x;
  sz_t y;
  sz_t z;
};

struct place_t
{
  place_t(pos_t pstart = {}, pos_t pend = {})  // NOLINT
      : start(pstart)
      , end(pend)
  {
  }

  place_t operator+(pos_t rhs) const { return {start + rhs, end + rhs}; }

  pos_t start;
  pos_t end;
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

class Layout
{
public:
  Layout() = default;

  Layout(const Layout&) = delete;
  Layout& operator=(const Layout&) = delete;

  Layout(Layout&&) = delete;
  Layout& operator=(Layout&&) = delete;

  virtual ~Layout() = default;

  const dim_t& dim() const { return m_dim; }
  dim_t& dim() { return m_dim; }

  virtual void resize(dim_t dim) = 0;
  virtual int render(pos_t pos) const = 0;

private:
  dim_t m_dim;
};

}  // namespace display
