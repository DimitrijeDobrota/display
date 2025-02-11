#pragma once

#include <memory>
#include <vector>

#include "display/screen.hpp"
#include "display/types.hpp"
#include "display/window.hpp"

namespace display
{

class LayoutFree : public Layout
{
public:
  using recalc_f = void(LayoutFree&);

  LayoutFree(recalc_f f_recalc)  // NOLINT
      : m_recalc(f_recalc)
  {
  }

  Window* operator[](std::size_t idx) { return m_windows[idx].get(); }
  const Window* operator[](std::size_t idx) const
  {
    return m_windows[idx].get();
  }

  template<typename T, class... Args>
  T& append(Args&&... args)
  {
    m_windows.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
    m_is_sorted = false;
    return *dynamic_cast<T*>(m_windows.back().get());
  }

  void resize(dim_t dim) override;
  int render(pos_t pos) const override;

private:
  recalc_f* m_recalc;

  std::vector<std::unique_ptr<Window>> m_windows;
  mutable bool m_is_sorted = true;
};

class LayoutRigid : public Layout
{
public:
  using recalc_f = void(LayoutRigid&);

  LayoutRigid(recalc_f f_recalc)  // NOLINT
      : m_recalc(f_recalc)
  {
  }

  auto& screen1() { return m_screen1; }
  auto& screen2() { return m_screen2; }

  void resize(dim_t dim) override;
  int render(pos_t pos) const override;

private:
  recalc_f* m_recalc;

  Screen m_screen1;
  Screen m_screen2;
};

}  // namespace display
