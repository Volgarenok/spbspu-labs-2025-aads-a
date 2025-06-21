#include "conversion.hpp"
#include <sstream>
#include <cmath>
#include <limits>
#include <utils/stack.hpp>
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
        size_t pos;
        std::stoll(token, std::addressof(pos));
        if (pos != token.length())
        {
          throw std::invalid_argument("Invalid token");
        }
        output.push(token);
      }
      catch (const std::invalid_argument &)
      {
        throw std::invalid_argument("Invalid token");
      }
      catch (const std::out_of_range &)
      {
        throw std::out_of_range("Number out of range");
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

long long smirnov::evaluatePostfix(Queue< std::string > & postfix)
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
      size_t pos;
      long long num = std::stoll(token, std::addressof(pos));
      if (pos != token.length())
      {
        throw std::invalid_argument("Invalid number");
      }
      values.push(num);
    }
    catch (const std::invalid_argument &)
    {
      if (values.empty())
      {
        throw std::invalid_argument("Not enough operands for operator");
      }
      long long b = values.drop();
      if (values.empty())
      {
        throw std::invalid_argument("Not enough operands for operator");
      }
      long long a = values.drop();
      long long result = 0;
      if (token == "+")
      {
        if ((a > 0 && b > max_val - a) || (a < 0 && b < min_val - a))
        {
          throw std::overflow_error("Addition overflow with");
        }
        result = a + b;
      }
      else if (token == "-")
      {
        if ((b > 0 && a < min_val + b) || (b < 0 && a > max_val + b))
        {
          throw std::overflow_error("Subtraction overflow with");
        }
        result = a - b;
      }
      else if (token == "*")
      {
        if (a > 0)
        {
          if (b > 0 && a > max_val / b)
          {
            throw std::overflow_error("Multiplication overflow with");
          }
          if (b < 0 && b < min_val / a)
          {
            throw std::overflow_error("Multiplication underflow with");
          }
        }
        else
        {
          if (b > 0 && a < min_val / b)
          {
            throw std::overflow_error("Multiplication underflow with");
          }
          if (b < 0 && a != 0 && b < max_val / a)
          {
            throw std::overflow_error("Multiplication overflow with");
          }
        }
        result = a * b;
      }
      else if (token == "/")
      {
        if (b == 0)
        {
          throw std::runtime_error("Division by zero with");
        }
        if (a == min_val && b == -1)
        {
          throw std::overflow_error("Division overflow with");
        }
        result = a / b;
      }
      else if (token == "%")
      {
        if (b == 0)
        {
          throw std::runtime_error("Modulo by zero with");
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
    catch (const std::out_of_range &)
    {
      throw std::out_of_range("Number out of range");
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
