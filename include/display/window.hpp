#pragma once

#include "display/element.hpp"
#include "display/types.hpp"

namespace display
{
class Window : public Element
{
public:
  explicit Window(place_t aplc, padd_t padd)
      : Element(aplc)
      , m_padd(padd)
  {
  }

  void render() const override;
  void clear() const override;
  void input(event& /* unused */) override {}

  void render_border() const;

protected:
  padd_t padd() const { return m_padd; }

  static std::ostream& set_cursor(sz_t posy, sz_t posx);
  std::ostream& line_next() const;

  void line_reset() const;
  void line_empty() const;
  void line_left(const std::string& text) const;
  void line_center(const std::string& text) const;
  void line_right(const std::string& text) const;

  place_t plc() const { return {pos(), dim()}; }
  pos_t pos() const { return {xpos(), ypos()}; }
  dim_t dim() const { return {wth(), hgt()}; }
  sz_t xpos() const { return axpos() + m_padd.left; }
  sz_t ypos() const { return aypos() + m_padd.top; }
  sz_t wth() const { return awth() - m_padd.width(); }
  sz_t hgt() const { return ahgt() - m_padd.height(); }

private:
  using Element::adim;
  using Element::ahgt;
  using Element::aplc;
  using Element::apos;
  using Element::awth;
  using Element::axpos;
  using Element::aypos;

  padd_t m_padd;

  mutable display::sz_t m_ypos = 0;
};

}  // namespace display
