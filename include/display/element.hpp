#pragma once

#include "display/types.hpp"

namespace display
{
class Element
{
public:
  explicit Element(place_t aplc)
      : m_aplc(aplc)
  {
  }

  Element(const Element&) = delete;
  Element& operator=(const Element&) = delete;

  Element(Element&&) = default;
  Element& operator=(Element&&) = default;

  virtual ~Element() = default;

  virtual void resize(place_t aplc) { m_aplc = aplc; }
  virtual void render() const = 0;
  virtual void clear() const = 0;
  virtual void input(event& evnt) = 0;

  static std::ostream& set_cursor(sz_t posy, sz_t posx);
  void render_border() const;

  const auto& aplc() const { return m_aplc; }
  const auto& apos() const { return aplc().pos; }
  const auto& adim() const { return aplc().dim; }
  const auto& axpos() const { return apos().x; }
  const auto& aypos() const { return apos().y; }
  const auto& awth() const { return adim().width; }
  const auto& ahgt() const { return adim().height; }

private:
  place_t m_aplc;
};

}  // namespace display
