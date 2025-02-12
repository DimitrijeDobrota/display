#pragma once

#include <optional>

#include "display/types.hpp"

namespace display
{

class Window
{
public:
  Window(pos_t pos = {})  // NOLINT
      : m_pos(pos)
  {
  }

  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;

  Window(Window&&) = default;
  Window& operator=(Window&&) = default;

  virtual ~Window() = default;

  const auto& pos() const { return m_pos; }
  auto& pos() { return m_pos; }

  virtual std::optional<place_t> place(dim_t bounds) = 0;
  virtual void render(place_t place) const = 0;

private:
  pos_t m_pos;
};

}  // namespace display
