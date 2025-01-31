#include <string>

#include "display/display.hpp"

exported_class::exported_class()
    : m_name {"display"}
{
}

auto exported_class::name() const -> char const*
{
  return m_name.c_str();
}
