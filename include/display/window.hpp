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

  virtual void resize(apos_t apos, dim_t dim) = 0;
  virtual void render() const = 0;
  virtual void input(event& /* unused */) {}
};

}  // namespace display
