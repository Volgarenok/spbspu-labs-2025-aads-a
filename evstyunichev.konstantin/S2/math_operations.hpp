#ifndef MATH_OPERATIONS
#define MATH_OPERATIONS
#include <cmath>
#include <limits>
#include <stdexcept>
namespace evstyunichev
{
  constexpr long long Maxll = std::numeric_limits< long long >::max();
  constexpr long long Minll = std::numeric_limits< long long >::min();
  long long summ(long long a, long long b, bool is_negative = 0);
  long long division(long long a, long long b);
  long long multipling(long long a, long long b);
  long long modul(long long a, long long b);
  long long concatinate(long long a, long long b);
}
#endif
