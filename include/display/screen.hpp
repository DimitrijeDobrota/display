#pragma once

#include <memory>

#include "display/types.hpp"

namespace display
{

class Screen
{
public:
  Screen(dim_t dim = {})  // NOLINT
      : m_dim(dim)
  {
  }

  Screen(const Screen&) = delete;
  Screen(Screen&&) = delete;

  Screen& operator=(const Screen&) = delete;
  Screen& operator=(Screen&&) = delete;

  ~Screen() = default;

  const auto& dim() const { return m_dim; }

  template<typename T, class... Args>
  T& set_layout(Args&&... args)
  {
    m_layout = std::make_unique<T>(std::forward<Args>(args)...);
    return *dynamic_cast<T*>(m_layout.get());
  }

  void resize(dim_t dim);
  void render(pos_t pos) const;

private:
  dim_t m_dim;

  std::unique_ptr<Layout> m_layout;
};

}  // namespace display
