#include <limits>

#include "display/utility.hpp"

#include "display/types.hpp"

int main()
{
  using namespace display;  // NOLINT
  using lim = std::numeric_limits<sz_t>;

  static constexpr const sz_t zero = 0;
  static constexpr const sz_t one = 1;

  // is_overflow
  static_assert(!is_overflow(lim::max(), zero));
  static_assert(is_overflow(lim::max(), one));
  static_assert(!is_overflow(lim::min(), zero));
  static_assert(!is_overflow(lim::min(), one));

  // is_underflow
  static_assert(!is_underflow(lim::max(), zero));
  static_assert(!is_underflow(lim::max(), one));
  static_assert(!is_underflow(lim::min(), zero));
  static_assert(is_underflow(lim::min(), one));

  // is_overflow_lim
  static_assert(!is_overflow_lim(1U, 0U, 2U));
  static_assert(is_overflow_lim(1U, 2U, 2U));
  static_assert(is_overflow_lim(3U, 1U, 2U));
  static_assert(is_overflow_lim(1U, 3U, 2U));

  // is_underflow_lim
  static_assert(!is_underflow_lim(2U, 1U, 1U));
  static_assert(is_underflow_lim(1U, 2U, 1U));
  static_assert(is_underflow_lim(0U, 1U, 1U));
  static_assert(is_underflow_lim(1U, 0U, 1U));

  // add_lim
  static_assert(add_lim(1U, 0U, 2U) == 1);
  static_assert(add_lim(1U, 2U, 2U) == 2);
  static_assert(add_lim(3U, 1U, 2U) == 2);
  static_assert(add_lim(1U, 3U, 2U) == 2);

  // sub_lim
  static_assert(sub_lim(3U, 1U, 1U) == 2);
  static_assert(sub_lim(1U, 2U, 1U) == 1);
  static_assert(sub_lim(0U, 1U, 1U) == 1);
  static_assert(sub_lim(1U, 0U, 1U) == 1);

  return 0;
}
