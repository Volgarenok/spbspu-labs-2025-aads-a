#ifndef CALC_UTILS_HPP
#define CALC_UTILS_HPP
#include <stdexcept>
#include <algorithm>

namespace demehin
{
  template< typename T >
  T getCheckedSum(const T& sum, const T& summand)
  {
    T max = std::numeric_limits< T >::max();
    if (sum > max - summand)
    {
      throw std::overflow_error("error: overflow");
    }
    return sum + summand;
  }
}


#endif
