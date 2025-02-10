#pragma once

#include <optional>

#include "display/layout.hpp"
#include "display/types.hpp"

namespace display
{

class Screen
{
public:
  Screen(dim_t dim)  // NOLINT
      : m_dim(dim)
  {
  }

  const auto& dim() const { return m_dim; }

  LayoutFree& set_layout(LayoutFree layout);

  void resize(dim_t new_dim);
  void render() const;

private:
  dim_t m_dim;

  std::optional<LayoutFree> m_layout;
};

}  // namespace display
