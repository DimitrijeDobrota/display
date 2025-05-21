#pragma once

#include "display/element.hpp"
#include "display/types.hpp"

namespace display
{

class Window : public Element
{
public:
  explicit Window(plc_t aplc, pad_t padd)
      : Element(aplc)
      , m_padd(padd)
  {
  }

  void render() const override;
  void clear() const override;
  void input(event& /* unused */) override {}

protected:
  [[nodiscard]] pad_t padd() const { return m_padd; }

  std::ostream& line_next() const;

  void line_reset() const;
  void line_empty() const;
  void line_left(const std::string& text) const;
  void line_center(const std::string& text) const;
  void line_right(const std::string& text) const;

  [[nodiscard]] plc_t plc() const { return {pos(), dim()}; }
  [[nodiscard]] pos_t pos() const { return {xpos(), ypos()}; }
  [[nodiscard]] dim_t dim() const { return {wth(), hgt()}; }
  [[nodiscard]] xpos_t xpos() const { return axpos() + m_padd.left; }
  [[nodiscard]] ypos_t ypos() const { return aypos() + m_padd.top; }
  [[nodiscard]] wth_t wth() const { return awth() - m_padd.width(); }
  [[nodiscard]] hgt_t hgt() const { return ahgt() - m_padd.height(); }

private:
  using Element::adim;
  using Element::ahgt;
  using Element::aplc;
  using Element::apos;
  using Element::awth;
  using Element::axpos;
  using Element::aypos;

  pad_t m_padd;

  mutable ypos_t m_ypos = ypos_t(0);
};

}  // namespace display
