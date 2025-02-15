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
      , m_apos(0, 0)
      , m_adim(0, 0)
  {
  }

  const auto& pos() const { return m_pos; }
  auto& pos() { return m_pos; }

  const auto& dim() const { return m_dim; }
  auto& dim() { return m_dim; }

  const auto& piv() const { return m_piv; }
  auto& piv() { return m_piv; }

  void resize(apos_t apos, dim_t dim) override;

protected:
  const auto& apos() const { return m_apos; }
  auto& apos() { return m_apos; }

  const auto& adim() const { return m_adim; }
  auto& adim() { return m_adim; }

private:
  pos_t m_pos;
  dim_t m_dim;
  piv_t m_piv;

  apos_t m_apos;
  dim_t m_adim;
};

}  // namespace display
