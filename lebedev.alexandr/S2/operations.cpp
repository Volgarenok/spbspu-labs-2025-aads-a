#include "operations.hpp"
#include <cmath>
#include <stdexcept>
#include <limits>

namespace
{
  long long add(long long a, long long b)
  {
    if (a > 0 && b > (std::numeric_limits< long long >::max() - a))
    {
      throw std::overflow_error("Overflow!\n");
    }
    else if (a < 0 && b < (std::numeric_limits< long long >::min() - a))
    {
      throw std::underflow_error("Overflow!\n");
    }
    return a + b;
  }

  long long substract(long long a, long long b)
  {
    if (b > 0)
    {
      if (a < (std::numeric_limits< long long >::min() + b))
      {
        throw std::underflow_error("Overflow!\n");
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

  long long multiply(long long a, long long b)
  {
    if (a > 0 && b > 0)
    {
      if (a > (std::numeric_limits< long long >::max() / b))
      {
        throw std::overflow_error("Overflow!\n");
      }
    }
    else if (a < 0 && b > 0)
    {
      if (a < (std::numeric_limits< long long >::min() / b))
      {
        throw std::underflow_error("Overflow!\n");
      }
    }
    else if (a > 0 && b < 0)
    {
      if (b < (std::numeric_limits< long long >::min() / a))
      {
        throw std::underflow_error("Overflow!\n");
      }
    }
    else if (a < 0 && b < 0)
    {
      if (a < (std::numeric_limits< long long >::max() / b))
      {
        throw std::overflow_error("Overflow!\n");
      }
    }
    return a * b;
  }

  long long divide(long long a, long long b)
  {
    if (b == 0)
    {
      throw std::logic_error("Division by zero!\n");
    }
    return a / b;
  }

  long long getRemainOfDivision(long long a, long long b)
  {
    if (b == 0)
    {
      throw std::logic_error("Division by zero!\n");
    }
    long long result = a % b;
    if (result < 0)
    {
      result += std::abs(b);
    }
    return result;
  }
}

long long lebedev::calculate(long long a, long long b, char sign)
{
  if (sign == '+')
  {
    return add(a, b);
  }
  else if (sign == '-')
  {
    return substract(a, b);
  }
  else if (sign == '*')
  {
    return multiply(a, b);
  }
  else if (sign == '/')
  {
    return divide(a, b);
  }
  else if (sign == '%')
  {
    return getRemainOfDivision(a, b);
  }
  throw std::invalid_argument("Unknown operation!\n");
}
