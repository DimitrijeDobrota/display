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

  void clear() const override;
  void input(event& /* unused */) override {}

protected:
  static dim_t padding() { return {0, 0}; }

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
  sz_t xpos() const { return axpos() + (m_padding.width / 2); }
  sz_t ypos() const { return aypos() + (m_padding.height / 2); }
  sz_t wth() const { return awth() - m_padding.width; }
  sz_t hgt() const { return ahgt() - m_padding.height; }

private:
  dim_t m_padding;

  mutable display::sz_t m_ypos = 0;
};

}  // namespace display
