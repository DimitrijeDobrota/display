#pragma once

#include "display/window.hpp"
#include "display/utility.hpp"

namespace display
{

class WindowPivot : public Window
{
public:
  WindowPivot(aplace_t aplc, piv_t piv, dim_t dim)
      : Window(aplc)
      , m_piv(piv)
      , m_dim(dim)
  {
  }

protected:
  aplace_t place() const
  {
    const auto [cols, rows] = adim();
    const sz_t colsh = cols / 2;
    const sz_t rowsh = rows / 2;

    const auto [wdth, hght] = m_dim;
    const sz_t wdthm = wdth / 2;
    const sz_t hghtm = hght / 2;

    const sz_t zero = 0;

    display::pos_t start;
    display::pos_t end;

    using display::add_lim, display::sub_lim;

    switch (m_piv.x) {
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

    switch (m_piv.y) {
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

    return {apos() + start, end - start};
  }

private:
  piv_t m_piv;
  dim_t m_dim;
};

}  // namespace display
