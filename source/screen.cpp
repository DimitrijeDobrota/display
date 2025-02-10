#include "display/screen.hpp"

#include "display/layout.hpp"

namespace display
{

LayoutFree& Screen::set_layout(LayoutFree layout)
{
  m_layout.emplace(std::move(layout));
  return m_layout.value();
}

void Screen::resize(dim_t new_dim)
{
  m_dim = new_dim;
  if (m_layout.has_value()) {
    m_layout->resize({}, m_dim);
  }
}

void Screen::render() const
{
  if (m_layout.has_value()) {
    m_layout->render();
  }
}

}  // namespace display
