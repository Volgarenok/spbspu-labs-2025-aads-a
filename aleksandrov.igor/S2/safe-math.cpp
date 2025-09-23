#include "safe-math.hpp"
#include <stdexcept>

namespace
{
  using aleksandrov::IntegralType;

  short int sign(IntegralType value)
  {
    return value > 0 ? 1 : (value < 0 ? -1 : 0);
  }

  bool hasSameSign(IntegralType a, IntegralType b)
  {
    return sign(a) * sign(b) > 0;
  }

  IntegralType abs(IntegralType value)
  {
    return value < 0 ? -value : value;
  }

  IntegralType euclidMod(IntegralType a, IntegralType b)
  {
    return (a % b + abs(b)) % abs(b);
  }
}

aleksandrov::IntegralType aleksandrov::safeSum(IntegralType a, IntegralType b)
{
  if (hasSameSign(a, b) && ((a > 0 && max - a < b) || (a < 0 && min - a > b)))
  {
    throw std::overflow_error("Addition overflow!");
  }
  return a + b;
}

aleksandrov::IntegralType aleksandrov::safeSub(IntegralType a, IntegralType b)
{
  return safeSum(a, -b);
}

aleksandrov::IntegralType aleksandrov::safeMul(IntegralType a, IntegralType b)
{
  if ((a == min && b == -1) || (b == min && a == -1) || abs(a) > max / abs(b))
  {
    throw std::overflow_error("Multiplication overflow!");
  }
  return a * b;
}

aleksandrov::IntegralType aleksandrov::safeDiv(IntegralType a, IntegralType b)
{
  if (!b)
  {
    throw std::invalid_argument("Division by zero!");
  }
  if (a == min && b == -1)
  {
    throw std::overflow_error("Division overflow!");
  }
  return a / b;
}

aleksandrov::IntegralType aleksandrov::safeMod(IntegralType a, IntegralType b)
{
  if (!b)
  {
    throw std::invalid_argument("Division with remainder by zero!");
  }
  return euclidMod(a, b);
}

