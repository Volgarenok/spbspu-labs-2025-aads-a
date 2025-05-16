#include "conversion_to_postfix.hpp"

std::string lanovenko::parseDigit(const std::string& str, size_t& num)
{
  std::string result = "";
  for (size_t i = num; i < str.length(); i++)
  {
    if (str[num] == ' ')
    {
      break;
    }
    else
    {
      result += str[i];
      num++;
    }
  }
  return result;
}

size_t lanovenko::getPriority(char c)
{
  size_t result = 0;
  if (c == '(')
  {
    result = 0;
  }
  if (c == ')')
  {
    result = 1;
  }
  if (c == '+' || c == '-')
  {
    result = 2;
  }
  if (c == '*' || c == '/' || c == '%')
  {
    result = 3;
  }
  return result;
}

void lanovenko::pushSign(Queue< std::string >& result, Stack< char >& operations, char sign)
{
  if (operations.empty())
  {
    operations.push(sign);
  }
  else if (!operations.empty())
  {
    while (!operations.empty() && getPriority(sign) <= getPriority(operations.top()))
    {
      result.push(operations.top() + std::string{ " " });
      operations.pop();
    }
    operations.push(sign);
  }
}

lanovenko::Queue< std::string > lanovenko::toPostfix(const std::string& infix)
{
  Queue< std::string > result;
  Stack< char > operations{};
  for (size_t i = 0; i < infix.size(); i++)
  {
    if (std::isdigit(infix[i]))
    {
      result.push(parseDigit(infix, i) + " ");
    }
    if (infix[i] == '+' || infix[i] == '-' || infix[i] == '*' || infix[i] == '/' || infix[i] == '%')
    {
      pushSign(result, operations, infix[i]);
    }
    if (infix[i] == '(')
    {
      operations.push(infix[i]);
    }
    if (infix[i] == ')')
    {
      while (!operations.empty() && operations.top() != '(')
      {
        result.push(operations.top() + std::string{ " " });
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
  }
  while (!operations.empty())
  {
    if (operations.top() == '(')
    {
      throw std::invalid_argument("incorrectly placed brackets!");
    }
    result.push(operations.top() + std::string{ " " });
    operations.pop();
  }
  if (result.empty())
  {
    throw std::invalid_argument("nothing to transorm");
  }
  return result;
}
