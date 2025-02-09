#pragma once

#include <alec/alec.hpp>

namespace display
{

using event = alec::event;

class Display
{
public:
  static Display& display();

  Display(const Display&) = delete;
  Display(Display&&) = delete;
  Display& operator=(const Display&) = delete;
  Display& operator=(Display&&) = delete;

  event get_event();

  bool get_resized() const;
  void set_resized();
  void reset_resized();

private:
  Display();
  ~Display();

  static void handle_sigwinch(int /* unused */);
  static bool is_resize_track;

  bool m_is_resized = false;
};

}  // namespace display
