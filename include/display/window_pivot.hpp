#pragma once

#include "display/window.hpp"

namespace display
{

place_t WindowPivot_place(place_t aplc,  // NOLINT
                          piv_t piv,
                          dim_t dim,
                          dim_t padding);

template<typename T>
  requires(std::is_base_of_v<Window, T>)
class WindowPivot : public T
{
public:
  WindowPivot(place_t aplc, piv_t piv, dim_t dim)
      : T(WindowPivot_place(aplc, piv, dim, T::padding()))
      , m_piv(piv)
      , m_dim(dim)
  {
  }

  void resize(place_t aplc) override
  {
    T::resize(WindowPivot_place(aplc, m_piv, m_dim, T::padding()));
  }

private:
  piv_t m_piv;
  dim_t m_dim;
};

}  // namespace display
