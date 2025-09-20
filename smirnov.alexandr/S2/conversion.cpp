#include "conversion.hpp"
#include <limits>
#include <utils/stack.hpp>
#include "utils.hpp"

smirnov::Queue< std::string > smirnov::infixToPostfix(const std::string & infix)
{
  Queue< std::string > output;
  Stack< std::string > opStack;
  List< std::string > tokens = splitBySpace(infix);
  for (auto it = tokens.begin(); it != tokens.end(); ++it)
  {
    const std::string & token = *it;
    if (token == "(")
    {
      opStack.push(token);
    }
    else if (token == ")")
    {
      while (!opStack.empty() && opStack.top() != "(")
      {
        output.push(opStack.top());
        opStack.pop();
      }
      if (opStack.empty())
      {
        throw std::invalid_argument("Mismatched parentheses");
      }
      opStack.pop();
    }
    else if (isOperator(token))
    {
      while (!opStack.empty() && opStack.top() != "(" && !comparePriority(opStack.top(), token))
      {
        output.push(opStack.top());
        opStack.pop();
      }
      opStack.push(token);
    }
    else
    {
      size_t pos = 0;
      try
      {
        std::stoll(token, std::addressof(pos));
      }
      catch (const std::exception &)
      {
        throw std::invalid_argument("Invalid token");
      }
      if (pos != token.length())
      {
        throw std::invalid_argument("Invalid token");
      }
      output.push(token);
    }
  }
  while (!opStack.empty())
  {
    if (opStack.top() == "(")
    {
      throw std::invalid_argument("Mismatched parentheses");
    }
    output.push(opStack.top());
    opStack.pop();
  }
  return output;
}

long long smirnov::evaluatePostfix(Queue< std::string > & postfix)
{
  Stack< long long > values;
  constexpr long long max_val = std::numeric_limits< long long >::max();
  constexpr long long min_val = std::numeric_limits< long long >::min();
  while (!postfix.empty())
  {
    std::string token = postfix.front();
    postfix.pop();
    size_t pos = 0;
    long long num = 0;
    bool is_number = true;
    try
    {
      num = std::stoll(token, std::addressof(pos));
    }
    catch (const std::out_of_range &)
    {
      throw std::out_of_range("Number out of range");
    }
    catch (...)
    {
      is_number = false;
    }
    if (is_number && pos == token.length())
    {
      values.push(num);
      continue;
    }
    if (!isOperator(token))
    {
      throw std::invalid_argument("Unknown operator");
    }
    if (values.size() < 2)
    {
      throw std::invalid_argument("Not enough operands for operator");
    }
    long long b = values.top(); values.pop();
    long long a = values.top(); values.pop();
    long long result = 0;
    if (token == "+")
    {
      if ((a > 0 && b > max_val - a) || (a < 0 && b < min_val - a))
      {
        throw std::overflow_error("Addition overflow");
      }
      result = a + b;
    }
    else if (token == "-")
    {
      if ((b > 0 && a < min_val + b) || (b < 0 && a > max_val + b))
      {
        throw std::overflow_error("Subtraction overflow");
      }
      result = a - b;
    }
    else if (token == "*")
    {
      if (a > 0)
      {
        if (b > 0 && a > max_val / b)
        {
          throw std::overflow_error("Multiplication overflow");
        }
        if (b < 0 && b < min_val / a)
        {
          throw std::overflow_error("Multiplication underflow");
        }
      }
      else
      {
        if (b > 0 && a < min_val / b)
        {
          throw std::overflow_error("Multiplication underflow");
        }
        if (b < 0 && a != 0 && b < max_val / a)
        {
          throw std::overflow_error("Multiplication overflow");
        }
      }
      result = a * b;
    }
    else if (token == "/")
    {
      if (b == 0)
      {
        throw std::runtime_error("Division by zero");
      }
      if (a == min_val && b == -1)
      {
        throw std::overflow_error("Division overflow");
      }
      result = a / b;
    }
    else if (token == "%")
    {
      if (b == 0)
      {
        throw std::runtime_error("Modulo by zero");
      }
      result = a % b;
      if (result < 0)
      {
        result += std::abs(b);
      }
    }
    values.push(result);
  }
  if (values.empty())
  {
    throw std::invalid_argument("No result");
  }
  long long result = values.top();
  values.pop();
  if (!values.empty())
  {
    throw std::invalid_argument("Too many operands");
  }
  return result;
}
