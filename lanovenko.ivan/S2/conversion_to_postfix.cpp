#include "conversion_to_postfix.hpp"

namespace
{
  size_t getPriority(const std::string& c)
  {
    size_t result = 0;
    if (c == "(")
    {
      result = 0;
    }
    if (c == ")")
    {
      result = 1;
    }
    if (c == "+" || c == "-")
    {
      result = 2;
    }
    if (c == "*" || c == "/" || c == "%")
    {
      result = 3;
    }
    return result;
  }
}

bool lanovenko::isOperator(const std::string& str)
{
  return (str == "+" || str == "-" || str == "*" || str == "/" || str == "%");
}

bool lanovenko::comparePriorities(const std::string& lhs, const std::string& rhs)
{
  return getPriority(lhs) <= getPriority(rhs);
}

void lanovenko::pushSign(Queue< std::string >& result, Stack< std::string >& operations, const std::string& sign)
{
  if (operations.empty())
  {
    operations.push(sign);
  }
  else if (!operations.empty())
  {
    while (!operations.empty() && comparePriorities(sign, operations.top()))
    {
      result.push(operations.top());
      operations.pop();
    }
    operations.push(sign);
  }
}

lanovenko::Queue< std::string > lanovenko::toPostfix(std::string infix)
{
  Queue< std::string > result;
  Stack< std::string > operations{};
  while (!infix.empty())
  {
    size_t space = infix.find(" ");
    std::string current = infix.substr(0, space);
    if (isOperator(current))
    {
      pushSign(result, operations, current);
    }
    else if (current == "(")
    {
      operations.push(current);
    }
    else if (current == ")")
    {
      while (!operations.empty() && operations.top() != "(")
      {
        result.push(operations.top());
        operations.pop();
      }
      if (!operations.empty())
      {
        operations.pop();
      }
      else
      {
        throw std::invalid_argument("incorrectly placed brackets!");
      }
    }
    else
    {
      result.push(current);
    }
    if (space == std::string::npos)
    {
      break;
    }
    infix = infix.substr(space + 1);
  }
  while (!operations.empty())
  {
    if (operations.top() == "(")
    {
      throw std::invalid_argument("incorrectly placed brackets!");
    }
    result.push(operations.top());
    operations.pop();
  }
  if (result.empty())
  {
    throw std::invalid_argument("nothing to transorm");
  }
  return result;
}
