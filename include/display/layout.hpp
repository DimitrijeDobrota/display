#pragma once

#include <cstdint>
#include <utility>
#include <vector>

namespace display
{

using sz_t = std::uint16_t;
using pos_t = std::pair<sz_t, sz_t>;
using dim_t = std::pair<sz_t, sz_t>;

class Window
{
public:
  Window(pos_t pos, dim_t dim)
      : m_pos(std::move(pos))
      , m_dim(std::move(dim))
  {
  }

  auto& pos() const { return m_pos; }
  auto xpos() const { return m_pos.first; }
  auto ypos() const { return m_pos.second; }

  auto& dim() const { return m_dim; }
  auto width() const { return m_dim.first; }
  auto height() const { return m_dim.second; }

  auto set_pos(pos_t pos) { m_pos = std::move(pos); }
  auto set_xpos(sz_t val) { m_pos.first = val; }
  auto set_ypos(sz_t val) { m_pos.second = val; }

  auto set_dim(dim_t dim) { m_dim = std::move(dim); }
  auto set_width(sz_t val) { m_dim.first = val; }
  auto set_height(sz_t val) { m_dim.second = val; }

private:
  pos_t m_pos;
  dim_t m_dim;
};

class LayoutFree
{
public:
  using render_f = int(Window&);

  auto operator[](std::size_t idx) const { return std::get<1>(m_windows[idx]); }
  auto operator[](std::size_t idx) { return std::get<1>(m_windows[idx]); }

  void append(Window window, int zidx = -1);

  int render(render_f renderer);

private:
  std::vector<std::pair<int, Window>> m_windows;
  bool m_is_sorted = true;
};

class LayoutRigid
{
public:
private:
};

}  // namespace display
