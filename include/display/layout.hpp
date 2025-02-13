#pragma once

#include "display/types.hpp"

namespace display
{

class Layout
{
public:
  Layout() = default;

  Layout(const Layout&) = delete;
  Layout& operator=(const Layout&) = delete;

  Layout(Layout&&) = delete;
  Layout& operator=(Layout&&) = delete;

  virtual ~Layout() = default;

  const dim_t& dim() const { return m_dim; }
  dim_t& dim() { return m_dim; }

  virtual void resize(dim_t dim) { m_dim = dim; }
  virtual void render(pos_t pos) const = 0;

private:
  dim_t m_dim;
};

}  // namespace display
