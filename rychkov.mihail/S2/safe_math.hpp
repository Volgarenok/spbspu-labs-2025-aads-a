#ifndef SAFE_MATH_HPP
#define SAFE_MATH_HPP

#include <type_traits>
#include <utility>
#include <limits>
#include <stdexcept>

namespace rychkov
{
  template< class T >
  std::enable_if_t< std::numeric_limits< T >::is_integer, bool > isAddOverflow(T num1, T num2)
  {
    if ((num1 >= 0) != (num2 >= 0))
    {
      return false;
    }
    if (num1 >= 0)
    {
      return std::numeric_limits< T >::max() - num2 < num1;
    }
    return std::numeric_limits< T >::min() - num1 > num2;
  }
  template< class T >
  std::enable_if_t< std::numeric_limits< T >::is_integer, bool > isSubOverflow(T num1, T num2)
  {
    if ((num1 >= 0) != (num2 < 0))
    {
      return false;
    }
    return (num2 == std::numeric_limits< T >::min()) || isAddOverflow< T >(num1, -num2);
  }
  template< class T >
  std::enable_if_t< std::numeric_limits< T >::is_integer, bool > isMulOverflow(T num1, T num2)
  {
    if ((num1 == 0) || (num2 == 0))
    {
      return false;
    }
    if (num1 > num2)
    {
      std::swap(num1, num2);
    }
    if (num1 >= 0)
    {
      return std::numeric_limits< T >::max() / num1 < num2;
    }
    if (num2 < 0)
    {
      return std::numeric_limits< T >::max() / num1 > num2;
    }
    return std::numeric_limits< T >::min() / num2 > num1;
  }
  template< class T >
  std::enable_if_t< std::numeric_limits< T >::is_integer, bool > isDivOverflow(T num1, T num2)
  {
    if (num2 == 0)
    {
      return true;
    }
    return (num1 == std::numeric_limits< T >::min()) && (num2 == -1);
  }
  template< class T >
  std::enable_if_t< std::numeric_limits< T >::is_integer, bool > isModOverflow(T, T num2)
  {
    return num2 == 0;
  }

  template< class T >
  T safeAdd(T num1, T num2)
  {
    if (isAddOverflow(num1, num2))
    {
      throw std::invalid_argument("sum overflow");
    }
    return num1 + num2;
  }
  template< class T >
  T safeSub(T num1, T num2)
  {
    if (isSubOverflow(num1, num2))
    {
      throw std::invalid_argument("sub overflow");
    }
    return num1 - num2;
  }
  template< class T >
  T safeMul(T num1, T num2)
  {
    if (isMulOverflow(num1, num2))
    {
      throw std::invalid_argument("mul overflow");
    }
    return num1 * num2;
  }
  template< class T >
  T safeDiv(T num1, T num2)
  {
    if (isDivOverflow(num1, num2))
    {
      throw std::invalid_argument("div overflow");
    }
    return num1 / num2;
  }
  template< class T >
  T safeMod(T num1, T num2)
  {
    if (isModOverflow(num1, num2))
    {
      throw std::invalid_argument("invalid mod");
    }
    T result = num1 % num2;
    if (result < 0)
    {
      if (num2 > 0)
      {
        result += num2;
      }
      else
      {
        result -= num2;
      }
    }
    return result;
  }
  template< class T >
  T executeOperation(T leftOperand, char operation, T rightOperand)
  {
    switch (operation)
    {
    case '*':
      return safeMul(leftOperand, rightOperand);
    case '/':
      return safeDiv(leftOperand, rightOperand);
    case '+':
      return safeAdd(leftOperand, rightOperand);
    case '-':
      return safeSub(leftOperand, rightOperand);
    case '%':
      return safeMod(leftOperand, rightOperand);
    }
    throw std::invalid_argument("invalid operation");
  }
}

#endif
