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
  pos_t(sz_t xpos = 0, sz_t ypos = 0)  // NOLINT
      : x(xpos)
      , y(ypos)
  {
  }

  pos_t operator+(pos_t rhs) const
  {
    return {
        static_cast<std::uint16_t>(x + rhs.x),
        static_cast<std::uint16_t>(y + rhs.y),
    };
  }

  sz_t x;
  sz_t y;
};

struct apos_t
{
  apos_t(sz_t xpos, sz_t ypos)
      : x(xpos)
      , y(ypos)
  {
  }

  apos_t operator+(pos_t rhs) const
  {
    return {
        static_cast<std::uint16_t>(x + rhs.x),
        static_cast<std::uint16_t>(y + rhs.y),
    };
  }

  sz_t x;
  sz_t y;
};

struct place_t
{
  place_t(pos_t pstart = {}, pos_t pend = {})  // NOLINT
      : start(pstart)
      , end(pend)
  {
  }

  place_t operator+(pos_t pos) const { return {start + pos, end + pos}; }

  pos_t start;
  pos_t end;
};

struct aplace_t
{
  aplace_t(apos_t astart, apos_t aend)  // NOLINT
      : start(astart)
      , end(aend)
  {
  }

  apos_t start;
  apos_t end;
};

inline aplace_t operator+(apos_t apos, place_t plc)
{
  return {apos + plc.start, apos + plc.end};
}

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

}  // namespace display
