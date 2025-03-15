#ifndef CALC_UTILS_HPP
#define CALC_UTILS_HPP
#include <stdexcept>
#include <algorithm>

namespace demehin
{
  template< typename T >
  T sumChecked(const T& sum, const T& summand)
  {
    static_assert(std::is_integral< T >::value);
    T max = std::numeric_limits< T >::max();
    if (sum > max - summand)
    {
      throw std::overflow_error("error: overflow");
    }
    return sum + summand;
  }
}


#endif
