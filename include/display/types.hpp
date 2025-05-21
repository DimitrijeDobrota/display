#pragma once

#include <cstdint>
#include <iostream>
#include <utility>

#include <alec/alec.hpp>
#include <alec/terminal.hpp>
#include <based/enum/enum.hpp>
#include <based/types/strong.hpp>
#include <based/types/types.hpp>

namespace display
{

using event = alec::event;

/* ----- Basic Types ----- */

struct xpos_t : based::strong_type<based::bu16, xpos_t>
{
  using strong_type::strong_type;
};

struct ypos_t : based::strong_type<based::bu16, ypos_t>
{
  using strong_type::strong_type;
};

struct wth_t : based::strong_type<based::bu16, wth_t>
{
  using strong_type::strong_type;
  explicit wth_t(std::size_t wth)
      : strong_type(static_cast<wth_t::basic_type>(wth))
  {
  }
};

struct hgt_t : based::strong_type<based::bu16, hgt_t>
{
  using strong_type::strong_type;
  explicit hgt_t(std::size_t hgt)
      : strong_type(static_cast<hgt_t::basic_type>(hgt))
  {
  }
};

auto compare(xpos_t, xpos_t) -> bool;
auto order(xpos_t, xpos_t) -> bool;
auto preinc(xpos_t) -> xpos_t;
auto add(xpos_t, xpos_t) -> xpos_t;
auto add(xpos_t, wth_t) -> xpos_t;
auto sub(xpos_t, xpos_t) -> wth_t;
auto sub(xpos_t, wth_t) -> xpos_t;
auto div(xpos_t, xpos_t) -> xpos_t;

auto compare(ypos_t, ypos_t) -> bool;
auto order(ypos_t, ypos_t) -> bool;
auto preinc(ypos_t) -> ypos_t;
auto add(ypos_t, ypos_t) -> ypos_t;
auto add(ypos_t, hgt_t) -> ypos_t;
auto sub(ypos_t, ypos_t) -> hgt_t;
auto sub(ypos_t, hgt_t) -> ypos_t;
auto div(ypos_t, ypos_t) -> ypos_t;

auto compare(wth_t, wth_t) -> bool;
auto order(wth_t, wth_t) -> bool;
auto preinc(wth_t) -> wth_t;
auto add(wth_t, wth_t) -> wth_t;
auto sub(wth_t, wth_t) -> wth_t;
auto mul(wth_t, wth_t) -> wth_t;
auto div(wth_t, wth_t) -> wth_t;

auto compare(hgt_t, hgt_t) -> bool;
auto order(hgt_t, hgt_t) -> bool;
auto preinc(hgt_t) -> hgt_t;
auto add(hgt_t, hgt_t) -> hgt_t;
auto sub(hgt_t, hgt_t) -> hgt_t;
auto mul(hgt_t, hgt_t) -> hgt_t;
auto div(hgt_t, hgt_t) -> hgt_t;

namespace literals
{

constexpr auto operator""_x(unsigned long long val)
{
  return xpos_t {static_cast<xpos_t::basic_type>(val)};
}

constexpr auto operator""_y(unsigned long long val)
{
  return ypos_t {static_cast<ypos_t::basic_type>(val)};
}

constexpr auto operator""_w(unsigned long long val)
{
  return wth_t {static_cast<wth_t::basic_type>(val)};
}

constexpr auto operator""_h(unsigned long long val)
{
  return hgt_t {static_cast<hgt_t::basic_type>(val)};
}

}  // namespace literals

struct dim_t
{
  dim_t(wth_t wdth, hgt_t hght)
      : width(wdth)
      , height(hght)
  {
  }

  dim_t(hgt_t hght, wth_t wdth)
      : width(wdth)
      , height(hght)
  {
  }

  dim_t(std::pair<based::bu16, based::bu16> pair)  // NOLINT
      : width(static_cast<wth_t::basic_type>(std::get<0>(pair)))
      , height(static_cast<hgt_t::basic_type>(std::get<1>(pair)))
  {
  }

  friend dim_t operator+(dim_t lhs, dim_t rhs)
  {
    return {lhs.width + rhs.width, lhs.height + rhs.height};
  }

  friend dim_t operator-(dim_t lhs, dim_t rhs)
  {
    return {lhs.width - rhs.width, lhs.height - rhs.height};
  }

  wth_t width;
  hgt_t height;
};

struct pad_t
{
  pad_t(wth_t width, hgt_t height)
      : pad_t(width, height, width, height)
  {
  }

  pad_t(hgt_t height, wth_t width)
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

  [[nodiscard]] wth_t width() const { return left + right; }
  [[nodiscard]] hgt_t height() const { return top + bottom; }

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
  pos_t(xpos_t xpos, ypos_t ypos)
      : x(xpos)
      , y(ypos)
  {
  }

  pos_t(ypos_t ypos, xpos_t xpos)
      : x(xpos)
      , y(ypos)
  {
  }

  friend pos_t operator+(pos_t lhs, pos_t rhs)
  {
    return {lhs.x + rhs.x, lhs.y + rhs.y};
  }

  friend dim_t operator-(pos_t lhs, pos_t rhs)
  {
    return {wth_t((lhs.x - rhs.x).value), hgt_t((lhs.y - rhs.y).value)};
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

  plc_t(dim_t dimval, pos_t posval)
      : pos(posval)
      , dim(dimval)
  {
  }

  pos_t pos;
  dim_t dim;
};

#define ENUM_PVTX left, center, right
#define ENUM_PVTY top, center, bottom

struct piv_t
{
  BASED_DECLARE_ENUM(x, based::bu8, 0, ENUM_PVTX)
  BASED_DECLARE_ENUM(y, based::bu8, 0, ENUM_PVTY)

  piv_t(x::enum_type pvtx, y::enum_type pvty)
      : piv_x(pvtx)
      , piv_y(pvty)
  {
  }

  piv_t(y::enum_type pvty, x::enum_type pvtx)
      : piv_x(pvtx)
      , piv_y(pvty)
  {
  }

  x::enum_type piv_x;
  y::enum_type piv_y;
};

BASED_DEFINE_ENUM_CLASS(piv_t, x, based::bu8, 0, ENUM_PVTX)
#undef ENUM_PVTX

BASED_DEFINE_ENUM_CLASS(piv_t, y, based::bu8, 0, ENUM_PVTY)
#undef ENUM_PVTY

}  // namespace display
