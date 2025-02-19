#pragma once

#include "display/window.hpp"

namespace display
{

class WindowPivot : public Window
{
public:
  WindowPivot(place_t aplc, padd_t padd, piv_t piv, dim_t dim)
      : Window(place(aplc, piv, dim + padd), padd)
      , m_piv(piv)
      , m_dim(dim)
  {
  }

  void resize(place_t aplc) override
  {
    Window::resize(place(aplc, m_piv, m_dim + padd()));
  }

private:
  static place_t place(place_t aplc, piv_t piv, dim_t dim);

  piv_t m_piv;
  dim_t m_dim;
};

}  // namespace display
