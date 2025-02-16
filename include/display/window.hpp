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

  void input(event& /* unused */) override {}
};

}  // namespace display
