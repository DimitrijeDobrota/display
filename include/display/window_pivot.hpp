#pragma once

#include "display/types.hpp"
#include "display/window.hpp"

namespace display
{

class WindowPivot : public Window
{
public:
  WindowPivot(aplace_t aplc, dim_t dim, piv_t piv)
      : Window(aplc)
      , m_piv(piv)
      , m_dim(dim)
  {
  }

  const auto& dim() const { return m_dim; }
  const auto& piv() const { return m_piv; }

  void set_pos(pos_t pos) { m_pos = pos; }

protected:
  std::tuple<apos_t, dim_t> place() const;

private:
  piv_t m_piv;
  dim_t m_dim;
  pos_t m_pos;
};

}  // namespace display
