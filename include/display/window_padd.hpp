#pragma once

#include "display/window.hpp"

namespace display
{

enum class WindowType : std::uint8_t
{
  Bare,
  Border,
  Box,
  BorderBox,
};

template<WindowType T>
class WindowPadd : public Window
{
public:
  explicit WindowPadd(place_t aplc)
      : Window(aplc, padding())
  {
  }

  void render() const override;

protected:
  static dim_t padding();

private:
  using Element::adim;
  using Element::ahgt;
  using Element::aplc;
  using Element::apos;
  using Element::awth;
  using Element::axpos;
  using Element::aypos;
};

template<>
inline dim_t WindowPadd<WindowType::Bare>::padding()
{
  return {0, 0};
}

template<>
inline dim_t WindowPadd<WindowType::Border>::padding()
{
  return {2, 2};
}

template<>
inline dim_t WindowPadd<WindowType::Box>::padding()
{
  return {2, 2};
}

template<>
inline dim_t WindowPadd<WindowType::BorderBox>::padding()
{
  return {4, 2};
}

template<>
void WindowPadd<WindowType::Bare>::render() const;

template<>
void WindowPadd<WindowType::Border>::render() const;

template<>
void WindowPadd<WindowType::Box>::render() const;

template<>
void WindowPadd<WindowType::BorderBox>::render() const;

}  // namespace display
