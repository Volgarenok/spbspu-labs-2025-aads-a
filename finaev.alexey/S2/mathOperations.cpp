#include "mathOperations.hpp"
#include <limits>
#include <stdexcept>
#include <cmath>

namespace
{
  long long countPlus(long long a, long long b)
  {
    if (a > 0 && b > (std::numeric_limits< long long >::max() - a))
    {
      throw std::overflow_error("Overflow!\n");
    }
    else if (a < 0 && b < (std::numeric_limits< long long >::min() + a))
    {
      throw std::overflow_error("Overflow!\n");
    }
    return a + b;
  }

  long long countMinus(long long a, long long b)
  {
    if (b > 0)
    {
      if (a < (std::numeric_limits< long long >::min() + b))
      {
        throw std::overflow_error("Underflow!\n");
      }
    }
    else if (b < 0)
    {
      if (a > (std::numeric_limits< long long >::max() + b))
      {
        throw std::overflow_error("Overflow!\n");
      }
    }
    return a - b;
  }

  long long countMult(long long a, long long b)
  {
    if (a > 0 && b > 0)
    {
      if (a > std::numeric_limits< long long >::max() / b)
      {
        throw std::overflow_error("Overflow!\n");
      }
    }
    else if (a < 0 && b < 0)
    {
      if (a < std::numeric_limits< long long >::max() / b)
      {
        throw std::overflow_error("Overflow!\n");
      }
    }
    else if (a > 0 && b < 0)
    {
      if (b < std::numeric_limits< int >::min() / a)
      {
        throw std::overflow_error("Overflow!\n");
      }
    }
    else if (a < 0 && b > 0)
    {
      if (a < std::numeric_limits< int >::min() / b)
      {
        throw std::overflow_error("Overflow!\n");
      }
    }
    return a * b;
  }

  long long countDiv(long long a, long long b)
  {
    if (b == 0)
    {
      throw std::logic_error("division by zero!\n");
    }
    return a / b;
  }

  long long countOstByDiv(long long a, long long b)
  {
    if (b == 0)
    {
      throw std::logic_error("division by zero!\n");
    }
    long long res = a % b;
    if (res < 0)
    {
      res += std::abs(b);
    }
    return res;
  }

  long long countExponentiation(long long a, long long b)
  {
    if (a == 0)
    {
      if (b < 0)
      {
        throw std::invalid_argument("Zero cannot be raised to a negative power!\n");
      }
      return 0;
    }
    if (a == 0 && b == 0)
    {
      throw std::invalid_argument("0^0 is undefined!");
    }
    if (a >= 0 && b >= 0)
    {
      long long res = 1;
      for (long long i = 0; i < b; ++i)
      {
        if (res > (std::numeric_limits< long long >::max() / a))
        {
          throw std::overflow_error("Overflow!\n");
        }
        res *= a;
      }
      return res;
    }
    else
    {
      if (a == 1)
      {
        return 1;
      }
      else if (a == -1)
      {
        if (std::abs(b) % 2 == 0)
        {
          return 1;
        }
        else
        {
          return -1;
        }
      }
      else
      {
        return 0;
      }
    }
  }
}

long long finaev::count(long long a, long long b, char sign)
{
  if (sign == '+')
  {
    return countPlus(a, b);
  }
  else if (sign == '-')
  {
    return countMinus(a, b);
  }
  else if (sign == '*')
  {
    return countMult(a, b);
  }
  else if (sign == '/')
  {
    return countDiv(a, b);
  }
  else if (sign == '%')
  {
    return countOstByDiv(a, b);
  }
  else if (sign == '^')
  {
    return countExponentiation(a, b);
  }
  throw std::invalid_argument("Unknown operation\n");
}
