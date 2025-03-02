#pragma once

#include "display/types.hpp"

namespace display
{
class Element
{
public:
  explicit Element(plc_t aplc)
      : m_aplc(aplc)
  {
  }

  Element(const Element&) = delete;
  Element& operator=(const Element&) = delete;

  Element(Element&&) = default;
  Element& operator=(Element&&) = delete;

  virtual ~Element() = default;

  virtual void resize(plc_t aplc) { m_aplc = aplc; }
  virtual void render() const = 0;
  virtual void clear() const = 0;
  virtual void input(event& evnt) = 0;

  static std::ostream& set_cursor(xpos_t xapos, ypos_t yapos);
  static std::ostream& set_cursor(pos_t apos);

  void render_border() const;

  plc_t aplc() const { return m_aplc; }
  pos_t apos() const { return aplc().pos; }
  dim_t adim() const { return aplc().dim; }
  xpos_t axpos() const { return apos().x; }
  ypos_t aypos() const { return apos().y; }
  wth_t awth() const { return adim().width; }
  hgt_t ahgt() const { return adim().height; }

private:
  plc_t m_aplc;
};

}  // namespace display
