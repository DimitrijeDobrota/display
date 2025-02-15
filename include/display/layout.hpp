#pragma once

#include "display/types.hpp"

namespace display
{

class Layout
{
public:
  explicit Layout(apos_t apos)
      : m_apos(apos)
  {
  }

  Layout(const Layout&) = delete;
  Layout& operator=(const Layout&) = delete;

  Layout(Layout&&) = default;
  Layout& operator=(Layout&&) = default;

  virtual ~Layout() = default;

  virtual void resize(apos_t apos, dim_t dim) { m_apos = apos, m_dim = dim; }
  virtual void render() const = 0;
  virtual void input(event& evnt) = 0;

protected:
  const dim_t& dim() const { return m_dim; }
  dim_t& dim() { return m_dim; }

  const apos_t& apos() const { return m_apos; }
  apos_t& apos() { return m_apos; }

private:
  apos_t m_apos;
  dim_t m_dim;
};

}  // namespace display
