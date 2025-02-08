#pragma once

#include <alec/alec.hpp>

namespace display
{

using exit_f = void();
using event = alec::event;

void exit();
void start(exit_f f_exit = exit);
void stop(bool force = false);

event get_event();

}  // namespace display
