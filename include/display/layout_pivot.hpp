#pragma once

#include <memory>

#include "display/layout.hpp"
#include "display/types.hpp"
#include "display/window_pivot.hpp"

namespace display
{

class LayoutPivot : public Layout<WindowPivot>
{
public:
  using ptr_t = std::unique_ptr<WindowPivot>;

  LayoutPivot(aplace_t aplc, piv_t piv)  // NOLINT
      : Layout(aplc)
      , m_piv(piv)
  {
  }

  template<typename M, class... Args>
  M& set_child(Args&&... args)
  {
    auto& child = Layout::set_child<M>(m_piv, std::forward<Args>(args)...);
    child.set_pos(get_pos());
    return child;
  }

  void resize(aplace_t aplc) override;

private:
  pos_t get_pos() const;

  piv_t m_piv;
};

}  // namespace display
