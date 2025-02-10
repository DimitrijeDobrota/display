#pragma once

#include <vector>

#include "display/window.hpp"

namespace display
{

class LayoutFree
{
public:
  using recalc_f = void(LayoutFree&);

  LayoutFree(recalc_f f_recalc)  // NOLINT
      : m_recalc(f_recalc)
  {
  }

  const pos_t& pos() const { return m_pos; }
  const dim_t& dim() const { return m_dim; }

  const auto& operator[](std::size_t idx) const { return m_windows[idx]; }
  auto& operator[](std::size_t idx) { return m_windows[idx]; }
  void append(Window window);

  void resize(pos_t pos, dim_t dim);
  int render() const;

private:
  recalc_f* m_recalc;
  pos_t m_pos;
  dim_t m_dim;

  std::vector<Window> m_windows;
  mutable bool m_is_sorted = true;
};

class LayoutRigid
{
public:
private:
};

}  // namespace display
