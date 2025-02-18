#pragma once

#include "display/element.hpp"
#include "display/types.hpp"

namespace display
{

class Window : public Element
{
public:
  using ustring = std::basic_string<unsigned char>;

  explicit Window(place_t aplc, dim_t padding = {0, 0})
      : Element(aplc)
      , m_padding(padding)
  {
  }

  void render() const override {}
  void clear() const override;
  void input(event& /* unused */) override {}

protected:
  static dim_t padding() { return {0, 0}; }

  std::ostream& set_cursor(sz_t posy, sz_t posx) const;
  std::ostream& line_next() const;

  void line_reset() const;
  void line_empty() const;
  void line_left(const std::string& text) const;
  void line_center(const std::string& text) const;
  void line_right(const std::string& text) const;

  place_t plc() const { return {pos(), dim()}; }
  pos_t pos() const { return {xpos(), ypos()}; }
  dim_t dim() const { return {wth(), hgt()}; }
  sz_t xpos() const { return axpos() + (m_padding.width / 2); }
  sz_t ypos() const { return aypos() + (m_padding.height / 2); }
  sz_t wth() const { return awth() - m_padding.width; }
  sz_t hgt() const { return ahgt() - m_padding.height; }

private:
  friend class WindowBorder;
  friend class WindowBox;
  friend class WindowBorderBox;

  using Element::adim;
  using Element::ahgt;
  using Element::aplc;
  using Element::apos;
  using Element::awth;
  using Element::axpos;
  using Element::aypos;

  dim_t m_padding;

  mutable display::sz_t m_ypos = 0;
};

class WindowBorder : public Window
{
public:
  explicit WindowBorder(place_t aplc)
      : Window(aplc, padding())
  {
  }

  void render() const override;

protected:
  static dim_t padding() { return {2, 2}; }
};

class WindowBox : public Window
{
public:
  explicit WindowBox(place_t aplc)
      : Window(aplc, padding())
  {
  }

  void render() const override;

protected:
  static dim_t padding() { return {2, 2}; }
};

class WindowBorderBox : public Window
{
public:
  explicit WindowBorderBox(place_t aplc)
      : Window(aplc, padding())
  {
  }

  void render() const override;

protected:
  static dim_t padding() { return {4, 2}; }
};

}  // namespace display
