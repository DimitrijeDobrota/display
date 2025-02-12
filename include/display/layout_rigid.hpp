#pragma once

#include <functional>

#include "display/screen.hpp"
#include "display/types.hpp"

namespace display
{

class LayoutRigid : public Layout
{
public:
  using recalc_f = std::function<void(LayoutRigid&)>;

  LayoutRigid(recalc_f f_recalc)  // NOLINT
      : m_recalc(std::move(f_recalc))
  {
  }

  auto& screen1() { return m_screen1; }
  auto& screen2() { return m_screen2; }

  void resize(dim_t dim) override;
  int render(pos_t pos) const override;

private:
  recalc_f m_recalc;

  Screen m_screen1;
  Screen m_screen2;
};

}  // namespace display
