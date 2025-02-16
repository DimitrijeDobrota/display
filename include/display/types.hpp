#pragma once

#include <cstdint>
#include <utility>

#include <alec/alec.hpp>

namespace display
{

using event = alec::event;

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
        static_cast<sz_t>(x + rhs.x),
        static_cast<sz_t>(y + rhs.y),
    };
  }

  dim_t operator-(pos_t rhs) const
  {
    return {static_cast<sz_t>(x - rhs.x), static_cast<sz_t>(y - rhs.y)};
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
        static_cast<sz_t>(x + rhs.x),
        static_cast<sz_t>(y + rhs.y),
    };
  }

  dim_t operator-(apos_t rhs) const
  {
    return {static_cast<sz_t>(rhs.x - x), static_cast<sz_t>(rhs.y - y)};
  }

  sz_t x;
  sz_t y;
};

struct place_t
{
  place_t(pos_t posval, dim_t dimval)
      : pos(posval)
      , dim(dimval)
  {
  }

  pos_t pos;
  dim_t dim;
};

struct aplace_t
{
  aplace_t(apos_t aposval, dim_t adimval)
      : apos(aposval)
      , adim(adimval)
  {
  }

  apos_t apos;
  dim_t adim;
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

}  // namespace display
