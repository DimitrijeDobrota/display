#pragma once

#include "display/window.hpp"

namespace display
{

class WindowPivot : public Window
{
public:
  WindowPivot(plc_t aplc, pad_t padd, piv_t piv, dim_t dim)
      : Window(place(aplc, piv, dim + padd), padd)
      , m_piv(piv)
      , m_dim(dim)
  {
  }

  void resize(plc_t aplc) override
  {
    Window::resize(place(aplc, m_piv, m_dim + padd()));
  }

private:
  static plc_t place(plc_t aplc, piv_t piv, dim_t dim);

  piv_t m_piv;
  dim_t m_dim;
};

}  // namespace display
