#pragma once

#include <optional>

#include "display/types.hpp"

namespace display
{

class Window
{
public:
  using render_f = void(const Window&, place_t place);

  Window(render_f frender, pos_t pos, dim_t dim, piv_t piv = {})
      : m_renderer(frender)
      , m_pos(pos)
      , m_dim(dim)
      , m_piv(piv)
  {
  }

  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;

  Window(Window&&) = default;
  Window& operator=(Window&&) = default;

  virtual ~Window() = default;

  const auto& pos() const { return m_pos; }
  auto& pos() { return m_pos; }

  const auto& dim() const { return m_dim; }
  auto& dim() { return m_dim; }

  const auto& piv() const { return m_piv; }
  auto& piv() { return m_piv; }

  virtual void render(place_t place) const;

  std::optional<place_t> place(dim_t bounds) const;

private:
  render_f* m_renderer;
  pos_t m_pos;
  dim_t m_dim;
  piv_t m_piv;
};

}  // namespace display
