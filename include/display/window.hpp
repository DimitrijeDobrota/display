#pragma once

#include <optional>

#include "display/types.hpp"

namespace display
{

class Window
{
public:
  Window() = default;

  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;

  Window(Window&&) = default;
  Window& operator=(Window&&) = default;

  virtual ~Window() = default;

  virtual std::optional<place_t> place(dim_t bounds) = 0;
  virtual void render(aplace_t place) const = 0;
};

}  // namespace display
