#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "display/layout.hpp"
#include "display/types.hpp"
#include "display/window.hpp"

namespace display
{

class LayoutFree : public Layout
{
public:
  using recalc_f = std::function<void(LayoutFree&)>;

  LayoutFree(recalc_f f_recalc)  // NOLINT
      : m_recalc(std::move(f_recalc))
  {
  }

  Window* operator[](std::size_t idx) { return m_wins[idx].get(); }
  const Window* operator[](std::size_t idx) const { return m_wins[idx].get(); }

  template<typename T, class... Args>
  T& append(Args&&... args)
  {
    m_wins.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
    return get<T>(m_wins.size() - 1);
  }

  template<typename T>
  const T& get(std::size_t idx) const
  {
    return *dynamic_cast<T*>(m_wins[idx].get());
  }

  template<typename T>
  T& get(std::size_t idx)
  {
    return *dynamic_cast<T*>(m_wins[idx].get());
  }

  void resize(dim_t dim) override;
  int render(pos_t pos) const override;

private:
  recalc_f m_recalc;

  std::vector<std::unique_ptr<Window>> m_wins;
};

}  // namespace display
