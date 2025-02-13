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

  void render(pos_t pos) const override;

private:
  std::vector<std::unique_ptr<Window>> m_wins;
};

}  // namespace display
