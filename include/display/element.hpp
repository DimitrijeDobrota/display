#pragma once

#include "display/types.hpp"

namespace display
{

class Element
{
public:
  explicit Element(aplace_t aplc)
      : m_aplc(aplc)
  {
  }

  Element(const Element&) = delete;
  Element& operator=(const Element&) = delete;

  Element(Element&&) = default;
  Element& operator=(Element&&) = default;

  virtual ~Element() = default;

  virtual void resize(aplace_t aplc) { m_aplc = aplc; }
  virtual void render() const = 0;
  virtual void input(event& evnt) = 0;

  const auto& aplc() const { return m_aplc; }
  const auto& apos() const { return aplc().apos; }
  const auto& adim() const { return aplc().adim; }
  const auto& axpos() const { return apos().x; }
  const auto& aypos() const { return apos().y; }
  const auto& awth() const { return adim().width; }
  const auto& ahgt() const { return adim().height; }

private:
  aplace_t m_aplc;
};

class ElementPlace : public Element
{
public:
  ElementPlace(aplace_t aplc, place_t plc)
      : Element(aplc)
      , m_plc(plc)
  {
  }

  const auto& plc() const { return m_plc; }
  const auto& pos() const { return plc().pos; }
  const auto& dim() const { return plc().dim; }
  const auto& xpos() const { return pos().x; }
  const auto& ypos() const { return pos().y; }
  const auto& wth() const { return dim().width; }

private:
  place_t m_plc;
};

}  // namespace display
