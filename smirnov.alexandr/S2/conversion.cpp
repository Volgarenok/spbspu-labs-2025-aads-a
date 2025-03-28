#include "conversion.hpp"
#include <sstream>
#include <cmath>
#include <limits>
#include "utils.hpp"


smirnov::Queue< std::string > smirnov::infixToPostfix(const std::string & infix)
{
  smirnov::Queue< std::string > output;
  Stack< std::string > opStack;
  std::istringstream iss(infix);
  std::string token;
  while (iss >> token)
  {
    if (token == "(")
    {
      opStack.push(token);
    }
    else if (token == ")")
    {
      while (!opStack.empty() && opStack.top() != "(")
      {
        output.push(opStack.drop());
      }
      if (opStack.empty())
      {
        throw std::invalid_argument("Mismatched parentheses");
      }
      opStack.drop();
    }
    else if (isOperator(token))
    {
      while (!opStack.empty() && opStack.top() != "(" && getPriority(opStack.top()) >= getPriority(token))
      {
        output.push(opStack.drop());
      }
      opStack.push(token);
    }
    else
    {
      try
      {
        std::stoll(token);
        output.push(token);
      }
      catch (...)
      {
        throw std::invalid_argument("Invalid token");
      }
    }
  }
  while (!opStack.empty())
  {
    if (opStack.top() == "(")
    {
      throw std::invalid_argument("Mismatched parentheses");
    }
    output.push(opStack.drop());
  }
  return output;
}

long long smirnov::evaluatePostfix(Queue<std::string> & postfix)
{
  Stack< long long > values;
  constexpr long long max_val = std::numeric_limits< long long >::max();
  constexpr long long min_val = std::numeric_limits< long long >::min();
  while (!postfix.empty())
  {
    std::string token = postfix.front();
    postfix.drop();
    try
    {
      long long num = std::stoll(token);
      values.push(num);
    }
    catch (...)
    {
      if (values.empty())
      {
        throw std::invalid_argument("Not enough operands");
      }
      long long b = values.drop();
      if (values.empty())
      {
        throw std::invalid_argument("Not enough operands");
      }
      long long a = values.drop();
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
          throw std::runtime_error("Division by zero");
        }
        result = a % b;
        if (result < 0)
        {
          result += std::abs(b);
        }
      }
      else
      {
        throw std::invalid_argument("Unknown operator");
      }
      values.push(result);
    }
  }
  if (values.empty())
  {
    throw std::invalid_argument("No result");
  }
  long long result = values.drop();
  if (!values.empty())
  {
    throw std::invalid_argument("Too many operands");
  }
  return result;
}

