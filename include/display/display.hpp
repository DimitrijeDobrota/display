#pragma once

#include <alec/alec.hpp>

#include "display/layout.hpp"

namespace display
{

class Display
{
public:
  static Display& display();

  Display(const Display&) = delete;
  Display(Display&&) = delete;
  Display& operator=(const Display&) = delete;
  Display& operator=(Display&&) = delete;

  const auto& layout() const { return m_layout; }
  auto& layout() { return m_layout; }

  event get_event();

  bool get_resized() const;
  void set_resized();
  void reset_resized();

  void render() const;
  void input(event& evnt);

private:
  Display();
  ~Display();

  void resize();

  static void handle_sigwinch(int /* unused */);
  static bool is_resize_track;

  Layout<Element> m_layout;
  bool m_is_resized = false;
};

}  // namespace display
