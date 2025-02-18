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
  dim_t()
      : dim_t(0, 0)
  {
  }

  dim_t(sz_t wdth, sz_t hght)  // NOLINT
      : width(wdth)
      , height(hght)
  {
  }

  dim_t(std::pair<sz_t, sz_t> pair)  // NOLINT
      : width(std::get<0>(pair))
      , height(std::get<1>(pair))
  {
  }

  dim_t operator+(dim_t rhs) const
  {
    return {
        static_cast<sz_t>(width + rhs.width),
        static_cast<sz_t>(height + rhs.height),
    };
  }

  sz_t width;
  sz_t height;
};

struct pos_t
{
  pos_t()
      : pos_t(0, 0)
  {
  }

  pos_t(sz_t xpos, sz_t ypos)
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
    return {
        static_cast<sz_t>(x - rhs.x),
        static_cast<sz_t>(y - rhs.y),
    };
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
  piv_t(PvtX pvtx, PvtY pvty)
      : x(pvtx)
      , y(pvty)
  {
  }

  PvtX x;
  PvtY y;
};

}  // namespace display
