#ifndef SAFE_MATH_HPP
#define SAFE_MATH_HPP

#include <limits>

namespace aleksandrov
{
  using IntegralType = long long int;

  constexpr IntegralType max = std::numeric_limits< IntegralType >::max();
  constexpr IntegralType min = std::numeric_limits< IntegralType >::min();

  IntegralType safeSum(IntegralType, IntegralType);
  IntegralType safeSub(IntegralType, IntegralType);
  IntegralType safeMul(IntegralType, IntegralType);
  IntegralType safeDiv(IntegralType, IntegralType);
  IntegralType safeMod(IntegralType, IntegralType);
}

#endif

