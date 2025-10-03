#ifndef CALC_UTILS_HPP
#define CALC_UTILS_HPP
#include <stdexcept>
#include <algorithm>
#include <limits>

namespace
{
  template< typename T >
  bool isMultipOverflow(const T& a, const T& b)
  {
    static_assert(std::is_integral< T >::value);
    T max = std::numeric_limits< T >::max();
    T min = std::numeric_limits< T >::min();
    if (a > 0 && b > 0 && a > max / b)
    {
      return true;
    }
    if (a < 0 && b < 0 && a < max / b)
    {
      return true;
    }
    if (a > 0 && b < 0 && b < min / a)
    {
      return true;
    }
    if (a < 0 && b > 0 && a < min / b)
    {
      return true;
    }
    return false;
  }
}

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

  template< typename T >
  T multipChecked(const T& a, const T& b)
  {
    static_assert(std::is_integral< T >::value);
    if (isMultipOverflow(a, b))
    {
      throw std::overflow_error("error: overflow");
    }
    return a * b;
  }

  template< typename T >
  T subtractChecked(const T& a, const T& b)
  {
    static_assert(std::is_integral< T >::value);
    T min = std::numeric_limits< T >::min();
    if (a < min + b)
    {
      throw std::overflow_error("error: underflow");
    }
    return a - b;
  }

  template< typename T >
  T modulateChecked(const T& a, const T& b)
  {
    static_assert(std::is_integral< T >::value);
    if (b == 0)
    {
      throw std::logic_error("zero modulation");
    }
    if (b < 0)
    {
      return -modulateChecked(-a, -b);
    }

    T res = a % b;
    if (res < 0)
    {
      res += b;
    }
    return res;
  }

  template< typename T >
  T divideChecked(const T& a, const T& b)
  {
    static_assert(std::is_integral< T >::value);
    if (b == 0)
    {
      throw std::logic_error("division by zero");
    }
    T min = std::numeric_limits< T >::min();
    if ((a == min && b == -1) || (b == min && a == -1))
    {
      throw std::logic_error("error: overflow");
    }

    return a / b;
  }
}


#endif
