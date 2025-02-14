#pragma once

#include <memory>

#include "display/layout.hpp"
#include "display/types.hpp"

namespace display
{

class Screen
{
public:
  explicit Screen(apos_t apos)
      : m_apos(apos)
  {
  }

  Screen(const Screen&) = delete;
  Screen& operator=(const Screen&) = delete;

  Screen(Screen&&) = default;
  Screen& operator=(Screen&&) = default;

  ~Screen() = default;

  template<typename T, class... Args>
  T& set_layout(Args&&... args)
  {
    m_layout = std::make_unique<T>(std::forward<Args>(args)...);
    return get_layout<T>();
  }

  template<typename T>
  const T& get_layout() const
  {
    return *dynamic_cast<T*>(m_layout.get());
  }

  template<typename T>
  T& get_layout()
  {
    return *dynamic_cast<T*>(m_layout.get());
  }

  bool has_layout() const { return m_layout != nullptr; }

  void resize(apos_t apos, dim_t dim);
  void render() const;

private:
  apos_t m_apos;
  dim_t m_dim;

  std::unique_ptr<Layout> m_layout;
};

}  // namespace display
