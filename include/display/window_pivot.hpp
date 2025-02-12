#pragma once

#include <functional>

#include "display/types.hpp"
#include "display/window.hpp"

namespace display
{

class WindowPivot : public Window
{
public:
  using render_f = std::function<void(const WindowPivot&, place_t place)>;

  WindowPivot(render_f frender, pos_t pos, dim_t dim, piv_t piv = {})
      : Window(pos)
      , m_renderer(std::move(frender))
      , m_dim(dim)
      , m_piv(piv)
  {
  }

  const auto& dim() const { return m_dim; }
  auto& dim() { return m_dim; }

  const auto& piv() const { return m_piv; }
  auto& piv() { return m_piv; }

  std::optional<place_t> place(dim_t bounds) override;
  void render(place_t place) const override;

private:
  render_f m_renderer;
  dim_t m_dim;
  piv_t m_piv;
};

}  // namespace display
