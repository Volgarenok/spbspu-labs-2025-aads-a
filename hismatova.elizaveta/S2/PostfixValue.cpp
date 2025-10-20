#include "PostfixValue.hpp"

namespace hismatova
{
  PostfixValue::PostfixValue():
    value(0),
    op(0),
    isNumber(true)
  {}
  PostfixValue::PostfixValue(int val):
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

  int PostfixValue::getValue() const
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
    return PostfixValue(value + rhs.getValue());
  }

  PostfixValue PostfixValue::operator-(const PostfixValue& rhs) const
  {
    return PostfixValue(value - rhs.getValue());
  }

  PostfixValue PostfixValue::operator*(const PostfixValue& rhs) const
  {
    return PostfixValue(value * rhs.getValue());
  }

  PostfixValue PostfixValue::operator/(const PostfixValue& rhs) const
  {
    if (rhs.getValue() == 0)
    {
      throw std::runtime_error("Division by zero");
    }
    return PostfixValue(value / rhs.getValue());
  }

  PostfixValue PostfixValue::operator%(const PostfixValue& rhs) const
  {
    if (rhs.getValue() == 0)
    {
      throw std::runtime_error("Modulo by zero");
    }
    return PostfixValue(value % rhs.getValue());
  }
}
