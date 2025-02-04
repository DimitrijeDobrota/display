#pragma once

namespace display
{

using exit_f = void();

void exit();
void start(exit_f f_exit = exit);
void stop(bool force = false);

int get_key();

}  // namespace display
