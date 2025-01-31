#include <cstdio>

#include "display/display.hpp"

int main()
{
  display::start();

  (void)std::getchar();

  display::stop();

  return 0;
}
