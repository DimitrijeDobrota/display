#pragma once

#include <vector>

#include "display/screen.hpp"
#include "display/types.hpp"

namespace display
{

class LayoutRigid : public Layout
{
public:
  using layout_t = std::vector<std::vector<std::uint8_t>>;

  LayoutRigid(layout_t layout);  // NOLINT

  const auto& operator[](std::size_t idx) const { return m_recs[idx].screen; }
  auto& operator[](std::size_t idx) { return m_recs[idx].screen; }

  void resize(dim_t dim) override;
  void render(pos_t pos) const override;

private:
  auto calc_width(dim_t share) const;
  auto calc_height(dim_t share) const;

  std::size_t count_and_pad(layout_t& layout) const;

  dim_t m_grid;

  struct record_t
  {
    Screen screen;
    dim_t start = {0xFFFF, 0xFFFF};
    dim_t dim;
  };

  std::vector<record_t> m_recs;
};

}  // namespace display
