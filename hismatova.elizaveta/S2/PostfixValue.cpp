#include <limits>
#include <cmath>
#include "PostfixValue.hpp"

namespace hismatova
{
  PostfixValue::PostfixValue():
    value(0),
    op(0),
    isNumber(true)
  {}
  PostfixValue::PostfixValue(int val):
    value(static_cast<long long>(val)),
    op(0),
    isNumber(true)
  {}
  PostfixValue::PostfixValue(long long val):
    value(val),
    op(0),
    isNumber(true)
  {}
  PostfixValue::PostfixValue(char oper):
    value(0),
    op(oper),
    isNumber(false)
  {}

  bool PostfixValue::isNumeric() const
  {
    return isNumber;
  }

  long long PostfixValue::getValue() const
  {
    if (!isNumber)
    {
      throw std::runtime_error("Not a number");
    }
    return value;
  }

  char PostfixValue::getOperator() const
  {
    if (isNumber)
    {
      throw std::runtime_error("Not an operator");
    }
    return op;
  }

  PostfixValue PostfixValue::operator+(const PostfixValue& rhs) const
  {
    long long result = value + rhs.getValue();
    if ((value > 0 && rhs.getValue() > 0 && result < 0) ||
      (value < 0 && rhs.getValue() < 0 && result > 0))
    {
      throw std::runtime_error("Int overflow");
    }
    return PostfixValue(result);
  }

  PostfixValue PostfixValue::operator-(const PostfixValue& rhs) const
  {
    long long result = value - rhs.getValue();
    if ((value > 0 && rhs.getValue() < 0 && result < 0) ||
      (value < 0 && rhs.getValue() > 0 && result > 0))
    {
      throw std::runtime_error("Int overflow");
    }
    return PostfixValue(result);
  }

  PostfixValue PostfixValue::operator*(const PostfixValue& rhs) const
  {
    long long result = value * rhs.getValue();
    if (value != 0 && result / value != rhs.getValue())
    {
      throw std::runtime_error("Int overflow");
    }
    return PostfixValue(result);
  }

  PostfixValue PostfixValue::operator/(const PostfixValue& rhs) const
  {
    if (rhs.getValue() == 0)
    {
      throw std::runtime_error("Division by zero");
    }
    if (value == LLONG_MIN && rhs.getValue() == -1)
    {
      throw std::runtime_error("Int overflow");
    }
    return PostfixValue(value / rhs.getValue());
  }

  PostfixValue PostfixValue::operator%(const PostfixValue& rhs) const
  {
    if (rhs.getValue() == 0)
    {
      throw std::runtime_error("Modulo by zero");
    }
    int result = value % rhs.getValue();
    if (result < 0)
    {
      result += std::abs(rhs.getValue());
    }
    return PostfixValue(static_cast< long long >(result));
  }
}
