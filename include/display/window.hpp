#pragma once

#include "display/element.hpp"
#include "display/types.hpp"

namespace display
{

class Window : public Element
{
public:
  explicit Window(aplace_t aplc)
      : Element(aplc)
  {
  }

  void clear() const override;
  void input(event& /* unused */) override {}

protected:
  std::ostream& next_line(bool reset = false) const;
  void line_empty(bool reset = false) const;
  void line_left(const std::string& text) const;
  void line_center(const std::string& text) const;
  void line_right(const std::string& text) const;

private:
  mutable display::sz_t m_ypos = 0;
};

}  // namespace display
