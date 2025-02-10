#pragma once

#include "display/types.hpp"

namespace display
{

class LayoutFree;

class Screen
{
public:
  Screen(dim_t dim)  // NOLINT
      : m_dim(dim)
  {
  }

  const auto& dim() const { return m_dim; }

  LayoutFree* get_layout() { return m_layout; }
  void set_layout(LayoutFree* layout);

  void resize(dim_t dim);

private:
  dim_t m_dim;

  LayoutFree* m_layout = nullptr;
};

}  // namespace display
