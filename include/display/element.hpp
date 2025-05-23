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
  Element& operator=(Element&&) = default;

  virtual ~Element() = default;

  virtual void resize(plc_t aplc) { m_aplc = aplc; }
  virtual void render() const = 0;
  virtual void clear() const = 0;
  virtual void input(event& evnt) = 0;

  static std::ostream& set_cursor(xpos_t xpos, ypos_t ypos);
  static std::ostream& set_cursor(pos_t pos);

  void render_border() const;

  [[nodiscard]] plc_t aplc() const { return m_aplc; }
  [[nodiscard]] pos_t apos() const { return aplc().pos; }
  [[nodiscard]] dim_t adim() const { return aplc().dim; }
  [[nodiscard]] xpos_t axpos() const { return apos().x; }
  [[nodiscard]] ypos_t aypos() const { return apos().y; }
  [[nodiscard]] wth_t awth() const { return adim().width; }
  [[nodiscard]] hgt_t ahgt() const { return adim().height; }

private:
  plc_t m_aplc;
};

}  // namespace display
