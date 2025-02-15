#pragma once

#include <memory>

#include "display/layout.hpp"
#include "display/types.hpp"
#include "display/window_pivot.hpp"

namespace display
{

class LayoutPivot : public Layout
{
public:
  using ptr_t = std::unique_ptr<WindowPivot>;

  LayoutPivot(apos_t apos, dim_t dim, piv_t piv)  // NOLINT
      : Layout(apos, dim)
      , m_piv(piv)
  {
  }

  template<typename T, class... Args>
  T& set_window(Args&&... args)
  {
    m_window = std::make_unique<T>(
        apos(), dim(), get_pos(), m_piv, std::forward<Args>(args)...);
    return get_window<T>();
  }

  template<typename T>
  const T& get_window() const
  {
    return *dynamic_cast<T*>(m_window.get());
  }

  template<typename T>
  T& get_window()
  {
    return *dynamic_cast<T*>(m_window.get());
  }

  void reset_window() { m_window.reset(); }

  bool has_window() const { return m_window != nullptr; }

  void resize(apos_t apos, dim_t dim) override;
  void render() const override;
  void input(event& evnt) override;

private:
  pos_t get_pos() const;

  piv_t m_piv;
  ptr_t m_window;
};

}  // namespace display
