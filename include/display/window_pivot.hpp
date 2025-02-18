#pragma once

#include "display/utility.hpp"
#include "display/window.hpp"

namespace display
{

class WindowPivot : public Window
{
public:
  WindowPivot(place_t aplc, piv_t piv, dim_t dim)
      : Window(place(aplc, piv, dim))
      , m_piv(piv)
      , m_dim(dim)
  {
  }

  void resize(place_t aplc) override
  {
    Window::resize(place(aplc, m_piv, m_dim));
  }

protected:
  static place_t place(place_t aplc, piv_t piv, dim_t dim)
  {
    const auto [cols, rows] = aplc.adim;
    const sz_t colsh = cols / 2;
    const sz_t rowsh = rows / 2;

    const auto [wdth, hght] = dim;
    const sz_t wdthm = wdth / 2;
    const sz_t hghtm = hght / 2;

    const sz_t zero = 0;

    pos_t start;
    pos_t end;

    using display::add_lim, display::sub_lim;

    switch (piv.x) {
      case PvtX::Left:
        start.x = 0;
        end.x = add_lim(start.x, wdth, cols);
        break;
      case PvtX::Center:
        start.x = sub_lim(colsh, wdthm, zero);
        end.x = add_lim(start.x, wdth, cols);
        break;
      case PvtX::Right:
        end.x = cols;
        start.x = sub_lim(end.x, wdth, zero);
        break;
    }

    switch (piv.y) {
      case PvtY::Top:
        start.y = 0;
        end.y = add_lim(start.y, hght, rows);
        break;
      case PvtY::Center:
        start.y = sub_lim(rowsh, hghtm, zero);
        end.y = add_lim(start.y, hght, rows);
        break;
      case PvtY::Bottom:
        end.y = rows;
        start.y = sub_lim(end.y, hght, zero);
        break;
    }

    return {aplc.apos + start, end - start};
  }

private:
  piv_t m_piv;
  dim_t m_dim;
};

}  // namespace display
