#pragma once

#include "display/types.hpp"

namespace display
{

class Window
{
public:
  Window(apos_t apos, dim_t dim)
      : m_apos(apos)
      , m_adim(dim)
  {
  }

  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;

  Window(Window&&) = default;
  Window& operator=(Window&&) = default;

  virtual ~Window() = default;

  virtual void resize(apos_t apos, dim_t adim) { m_apos = apos, m_adim = adim; }
  virtual void render() const = 0;
  virtual void input(event& /* unused */) {}

protected:
  const auto& apos() const { return m_apos; }
  auto& apos() { return m_apos; }

  const auto& adim() const { return m_adim; }
  auto& adim() { return m_adim; }

private:
  apos_t m_apos;
  dim_t m_adim;
};

}  // namespace display
