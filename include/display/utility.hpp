#pragma once

#include <algorithm>

namespace display
{

template<typename T>
inline bool is_oveflow_lim(T val, T add, T lim)
{
  return val > lim || add > lim || val > lim - add;
}

template<typename T>
inline bool is_underflow_lim(T val, T sub, T lim)
{
  return val < lim || sub < lim || val < lim + sub;
}

template<typename T>
inline bool is_oveflow(T val, T add)
{
  return val > std::numeric_limits<T>::max() - add;
}

template<typename T>
inline bool is_underflow(T val, T sub)
{
  return val < std::numeric_limits<T>::min() + sub;
}

template<typename T>
inline T add_lim(T val, T add, T lim)
{
  return !is_oveflow_lim(val, add, lim) ? val + add : lim;
}

template<typename T>
inline T sub_lim(T val, T sub, T lim)
{
  return !is_underflow_lim(val, sub, lim) ? val - sub : lim;
}

template<typename T>
inline T clamp_low(T val, T low)
{
  return std::max(val, low);
}

template<typename T>
inline T clamp_high(T val, T high)
{
  return std::min(val, high);
}

template<typename T>
inline T clamp(T val, T low, T high)
{
  return std::max(std::min(val, high), low);
}

}  // namespace display
