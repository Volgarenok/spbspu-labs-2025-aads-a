#include <limits>
#include "calculationProcess.hpp"
#include "stack.hpp"

namespace
{
  bool isNumber(const std::string& symbol)
  {
    try
    {
      std::stoll(symbol);
    }
    catch (const std::exception&)
    {
      return false;
    }
    return true;
  }

  bool isOperator(const std::string& symbol)
  {
    return (symbol == "+" || symbol == "-" || symbol == "/" || symbol == "%" || symbol == "*" || symbol == "**");
  }

  int calculatePriority(const std::string& operation)
  {
    if (operation == "+" || operation == "-")
    {
      return 1;
    }
    if (operation == "*" || operation == "/" || operation == "%")
    {
      return 2;
    }
    if (operation == "**")
    {
      return 3;
    }
    return 0;
  }

  long long int calculateOperation(const std::string& operation, long long int operand1, long long int operand2)
  {
    const long long int min = std::numeric_limits< long long int >::min();
    const long long int max = std::numeric_limits< long long int >::max();
    if (operation == "+")
    {
      if ((operand2 > 0) && (operand1 > (max - operand2)))
      {
        throw std::runtime_error("Addition overflow!");
      }
      if ((operand2 < 0) && (operand1 < (min - operand2)))
      {
        throw std::runtime_error("Addition underflow!");
      }
      return operand1 + operand2;
    }
    else if (operation == "-")
    {
      if ((operand2 > 0) && (operand1 < (min + operand2)))
      {
        throw std::runtime_error("Subtraction underflow!");
      }
      if ((operand2 < 0) && (operand1 > (max + operand2)))
      {
        throw std::runtime_error("Subtraction overflow!");
      }
      return operand1 - operand2;
    }
    else if (operation == "/")
    {
      if (operand2 == 0)
      {
        throw std::runtime_error("Division by zero!");
      }
      if ((operand1 == min) && (operand2 == -1))
      {
        throw std::runtime_error("Division overflow!");
      }
      return operand1 / operand2;
    }
    else if (operation == "%")
    {
      if (operand2 == 0)
      {
        throw std::runtime_error("Division by zero!");
      }
      return (operand1 % operand2 + operand2) % operand2;
    }
    else if (operation == "*")
    {
      if (operand1 > 0)
      {
        if (operand2 > 0)
        {
          if (operand1 > max / operand2)
          {
            throw std::runtime_error("Multiplication overflow!");
          }
          else if (operand2 < min / operand1)
          {
            throw std::runtime_error("Multiplication underflow!");
          }
        }
      }
      else if (operand1 < 0)
      {
        if (operand2 > 0)
        {
          if (operand1 < min / operand2)
          {
            throw std::runtime_error("Multiplication underflow!");
          }
          else if (operand2 != 0 && operand1 > max / operand2)
          {
            throw std::runtime_error("Multiplication overflow!");
          }
        }
      }
      return operand1 * operand2;
    }
    else if (operation == "**")
    {
      if (operand1 == 0)
      {
        if (operand2 <= 0)
        {
          throw std::runtime_error("Undefined power!");
        }
        else
        {
          return 0;
        }
      }
      else if (operand2 < 0)
      {
        throw std::runtime_error("Negative power!");
      }
      else if (operand2 == 0 || operand1 == 1)
      {
        return 1;
      }
      if (operand1 == -1)
      {
        return (operand2 % 2 == 0) ? 1 : -1;
      }
      bool isNegative = (operand1 < 0) && (operand2 % 2 != 0);
      operand1 = std::abs(operand1);
      long long int result = 1;
      for (long long int i = 0; i < operand2; ++i)
      {
        if (result > max / operand1)
        {
          throw std::runtime_error("Power overflow!");
        }
        result *= operand1;
      }
      return isNegative ? -result : result;
    }
    else
    {
      throw std::runtime_error("Wrong operation!");
    }
  }
}

kushekbaev::Queue< std::string > kushekbaev::convertToPostfix(Queue< std::string > Q)
{
  Queue< std::string > postfixQ;
  Stack< std::string > stack;
  std::string symbol;
  while (!Q.empty())
  {
    symbol = Q.front();
    if (symbol == "(")
    {
      stack.push(symbol);
    }
    else if (isNumber(symbol))
    {
      postfixQ.push(symbol);
    }
    else if (isOperator(symbol))
    {
      int priority = calculatePriority(symbol);
      while (!stack.empty() && priority <= calculatePriority(stack.top()))
      {
        postfixQ.push(stack.top());
        stack.pop();
      }
      stack.push(symbol);
    }
    else if (symbol == ")")
    {
      while (!stack.empty() && stack.top() != "(")
      {
        postfixQ.push(stack.top());
        stack.pop();
      }
      if (!stack.empty() && stack.top() == "(")
      {
        stack.pop();
      }
      else
      {
        throw std::runtime_error("Wrong parenthesis input!");
      }
    }
    else
    {
      throw std::runtime_error("Invalid symbol!");
    }
    Q.pop();
  }
  while (!stack.empty())
  {
    postfixQ.push(stack.top());
    stack.pop();
  }
  return postfixQ;
}

long long int kushekbaev::calculatePostfix(Queue< std::string > postfixQ)
{
  Stack< long long int > stack;
  while (!postfixQ.empty())
  {
    std::string symbol = postfixQ.front();
    postfixQ.pop();
    if (isNumber(symbol))
    {
      stack.push(std::stoll(symbol));
    }
    else if (isOperator(symbol))
    {
      if (stack.size() < 2)
      {
        throw std::runtime_error("Unary operation demands two operands!");
      }
      long long int operand2 = stack.top();
      stack.pop();
      long long int operand1 = stack.top();
      stack.pop();
      stack.push(calculateOperation(symbol, operand1, operand2));
    }
  }
  long long int result  = stack.top();
  stack.pop();
  return result;
}
