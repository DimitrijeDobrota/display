#pragma once

#include <optional>

#include "display/types.hpp"

namespace display
{

class LayoutFree;

class Window
{
public:
  Window(pos_t pos, dim_t dim, piv_t piv = {})
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

  LayoutFree* get_layout() { return m_layout; }
  void set_layout(LayoutFree* layout) { m_layout = layout; }

  std::optional<place_t> place() const;

private:
  pos_t m_pos;
  dim_t m_dim;
  piv_t m_piv;

  LayoutFree* m_layout = nullptr;
};

}  // namespace display
