#pragma once

#include "display/utility.hpp"
#include "display/window.hpp"

namespace display
{

template<typename T>
  requires(std::is_base_of_v<Window, T>)
class WindowPivot : public T
{
public:
  WindowPivot(place_t aplc, piv_t piv, dim_t dim)
      : T(place(aplc, piv, dim, T::padding()))
      , m_piv(piv)
      , m_dim(dim)
  {
  }

  void resize(place_t aplc) override
  {
    T::resize(place(aplc, m_piv, m_dim, T::padding()));
  }

protected:
  static place_t place(place_t aplc, piv_t piv, dim_t dim, dim_t padding)
  {
    const auto [cols, rows] = aplc.dim;
    const sz_t colsh = cols / 2;
    const sz_t rowsh = rows / 2;

    const auto [wdth, hght] = dim + padding;
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

    return {aplc.pos + start, end - start};
  }

private:
  piv_t m_piv;
  dim_t m_dim;
};

}  // namespace display
