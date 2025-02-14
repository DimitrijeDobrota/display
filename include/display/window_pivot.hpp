#pragma once

#include "display/types.hpp"
#include "display/window.hpp"

namespace display
{

class WindowPivot : public Window
{
public:
  WindowPivot(pos_t pos, dim_t dim, piv_t piv = {})
      : m_pos(pos)
      , m_dim(dim)
      , m_piv(piv)
  {
  }

  const auto& pos() const { return m_pos; }
  auto& pos() { return m_pos; }

  const auto& dim() const { return m_dim; }
  auto& dim() { return m_dim; }

  const auto& piv() const { return m_piv; }
  auto& piv() { return m_piv; }

  std::optional<place_t> place(dim_t bounds) override;
  void render(aplace_t place) const override = 0;

private:
  pos_t m_pos;
  dim_t m_dim;
  piv_t m_piv;
};

}  // namespace display
