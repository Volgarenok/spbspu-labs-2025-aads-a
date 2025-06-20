#include "expression_calculations.hpp"
#include "conversion_to_postfix.hpp"

bool lanovenko::additionOverflow(long long a, long long b)
{
  return (b > std::numeric_limits< long long >::max() - a);
}

bool lanovenko::multiplicationOverflow(long long a, long long b)
{
  if (a == 0 || b == 0)
  {
    return false;
  }
  if (a > 0)
  {
    if (b > 0)
    {
      return a > std::numeric_limits< long long >::max() / b;
    }
    else
    {
      return b < std::numeric_limits< long long >::min() / a;
    }
  }
  else
  {
    if (b > 0)
    {
      return a < std::numeric_limits< long long >::min() / b;
    }
    else
    {
      return a < std::numeric_limits< long long >::max() / b;
    }
  }
}

long long lanovenko::calculateBinary(long long a, long long b, const std::string& operation)
{
  if (operation == "+")
  {
    if (additionOverflow(a, b))
    {
      throw std::logic_error("Sum overflow");
    }
    return a + b;
  }
  if (operation == "-")
  {
    return a - b;
  }
  if (operation == "*")
  {
    if (multiplicationOverflow(a, b))
    {
      throw std::logic_error("Multiplication overflow");
    }
    return a * b;
  }
  if (operation == "/")
  {
    if (b == 0)
    {
      throw std::invalid_argument("Division by zero!");
    }
    return (a / b);
  }
  if (operation == "%")
  {
    if (b == 0)
    {
      throw std::invalid_argument("Division by zero!");
    }
    return ((a % b) + b) % b;
  }
  return 0;
}

void lanovenko::pushCalculated(Stack< long long >& result, const std::string& operation)
{
  if (result.empty())
  {
    throw std::invalid_argument("Wrong expression");
  }
  long long b = result.top();
  result.pop();
  if (result.empty())
  {
    throw std::invalid_argument("Wrong expression");
  }
  long long a = result.top();
  result.pop();
  result.push(calculateBinary(a, b, operation));
}

long long lanovenko::calcualtePostfix(Queue< std::string >& postfix)
{
  Stack< long long > result{};
  while (!postfix.empty())
  {
    std::string current = postfix.front();
    if (isOperator(current))
    {
      pushCalculated(result, current);
    }
    else
    {
      result.push(std::stoll(current));
    }
    postfix.pop();
  }
  long long res = result.top();
  result.pop();
  return res;
}
