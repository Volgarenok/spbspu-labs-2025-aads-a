#ifndef TEST_COMMON_HPP
#define TEST_COMMON_HPP
#include <cmath>

namespace savintsev
{
  bool divisible_by_five(const int value)
  {
    return (value % 2);
  }
  struct is_even
  {
    bool operator()(const int value)
    {
      return (value % 2) == 0;
    }
  };
  bool mycomparison(double first, double second)
  {
    return int(first) < int(second);
  }
  bool same_integral_part(double first, double second)
  {
    return int(first) == int(second);
  }
  struct is_near
  {
    bool operator()(double first, double second)
    {
      return fabs(first - second) < 5.0;
    }
  };
}

#endif
