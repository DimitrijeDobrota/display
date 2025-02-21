#pragma once

#include <cstdint>
#include <iostream>
#include <utility>

#include <alec/alec.hpp>

namespace display
{

using event = alec::event;

template<char C>
struct val_t
{
  using sz_t = std::uint16_t;

  explicit val_t(std::size_t xpos)
      : v(static_cast<sz_t>(xpos))
  {
  }

  explicit val_t(int xpos)
      : v(static_cast<sz_t>(xpos))
  {
  }

  explicit val_t(sz_t xpos)  // NOLINT
      : v(xpos)
  {
  }

  auto value() const { return v; }
  auto& value() { return v; }

  // clang-format off
  val_t operator+(val_t rhs) const { return val_t(static_cast<sz_t>(v + rhs.v)); }
  val_t operator-(val_t rhs) const { return val_t(static_cast<sz_t>(v - rhs.v)); }
  val_t operator*(val_t rhs) const { return val_t(static_cast<sz_t>(v * rhs.v)); }
  val_t operator/(val_t rhs) const { return val_t(static_cast<sz_t>(v / rhs.v)); }

  val_t operator+(int rhs) const { return val_t(static_cast<sz_t>(v + rhs)); }
  val_t operator-(int rhs) const { return val_t(static_cast<sz_t>(v - rhs)); }
  val_t operator*(int rhs) const { return val_t(static_cast<sz_t>(v * rhs)); }
  val_t operator/(int rhs) const { return val_t(static_cast<sz_t>(v / rhs)); }

  auto& operator+=(val_t rhs) { v += rhs.v; return *this; }
  auto& operator-=(val_t rhs) { v -= rhs.v; return *this; }
  auto& operator*=(val_t rhs) { v *= rhs.v; return *this; }
  auto& operator/=(val_t rhs) { v /= rhs.v; return *this; }

  auto& operator+=(int rhs) { v += rhs; return *this; }
  auto& operator-=(int rhs) { v -= rhs; return *this; }
  auto& operator*=(int rhs) { v *= rhs; return *this; }
  auto& operator/=(int rhs) { v /= rhs; return *this; }

  auto& operator++() { return *this += 1; }
  auto& operator--() { return *this -= 1; }

  val_t operator++(int) { return val_t(v++); }
  val_t operator--(int) { return val_t(v--); }

  auto operator<=>(const val_t&) const = default;

  friend std::ostream& operator<<(std::ostream& ost, val_t cord) {
      return ost << cord.value();
  }

  // clang-format on

private:
  sz_t v;  // NOLINT
};

using xpos_t = val_t<'x'>;
using ypos_t = val_t<'y'>;
using wth_t = val_t<'w'>;
using hgt_t = val_t<'h'>;

// clang-format off
inline xpos_t operator+(xpos_t lhs, wth_t rhs) { return xpos_t(lhs.value() + rhs.value()); }
inline xpos_t operator-(xpos_t lhs, wth_t rhs) { return xpos_t(lhs.value() - rhs.value()); }
inline ypos_t operator+(ypos_t lhs, hgt_t rhs) { return ypos_t(lhs.value() + rhs.value()); }
inline ypos_t operator-(ypos_t lhs, hgt_t rhs) { return ypos_t(lhs.value() - rhs.value()); }
// clang-format on

struct dim_t
{
  dim_t(int wdth, int hght)  // NOLINT
      : width(wdth)
      , height(hght)
  {
  }

  dim_t(wth_t wdth, hgt_t hght)  // NOLINT
      : width(wdth)
      , height(hght)
  {
  }

  dim_t(std::pair<std::uint16_t, std::uint16_t> pair)  // NOLINT
      : width(std::get<0>(pair))
      , height(std::get<1>(pair))
  {
  }

  dim_t operator+(dim_t rhs) const
  {
    return {width + rhs.width, height + rhs.height};
  }

  dim_t operator-(dim_t rhs) const
  {
    return {width - rhs.width, height - rhs.height};
  }

  wth_t width;
  hgt_t height;
};

struct pad_t
{
  pad_t(int width, int height)  // NOLINT
      : pad_t(width, height, width, height)
  {
  }

  pad_t(int leftpos,  // NOLINT
        int toppos,
        int rightpos,
        int bottompos)
      : left(leftpos)
      , right(rightpos)
      , top(toppos)
      , bottom(bottompos)
  {
  }

  pad_t(wth_t width, hgt_t height)  // NOLINT
      : pad_t(width, height, width, height)
  {
  }

  pad_t(wth_t leftpos, hgt_t toppos, wth_t rightpos, hgt_t bottompos)
      : left(leftpos)
      , right(rightpos)
      , top(toppos)
      , bottom(bottompos)
  {
  }

  wth_t width() const { return left + right; }
  hgt_t height() const { return top + bottom; }

  friend dim_t operator+(dim_t dim, pad_t rhs)
  {
    return {dim.width + rhs.width(), dim.height + rhs.height()};
  }

  wth_t left;
  wth_t right;
  hgt_t top;
  hgt_t bottom;
};

struct pos_t
{
  using sz_t = std::uint16_t;

  pos_t(int xpos, int ypos)
      : x(xpos)
      , y(ypos)
  {
  }

  pos_t(sz_t xpos, sz_t ypos)
      : x(xpos)
      , y(ypos)
  {
  }

  pos_t(xpos_t xpos, ypos_t ypos)
      : x(xpos)
      , y(ypos)
  {
  }

  pos_t operator+(pos_t rhs) const { return {x + rhs.x, y + rhs.y}; }
  dim_t operator-(pos_t rhs) const
  {
    return {wth_t((x - rhs.x).value()), hgt_t((y - rhs.y).value())};
  }

  pos_t operator+(dim_t rhs) const { return {x + rhs.width, y + rhs.height}; }

  xpos_t x;
  ypos_t y;
};

struct plc_t
{
  plc_t(pos_t posval, dim_t dimval)
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
