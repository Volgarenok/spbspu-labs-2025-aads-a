#include <limits>
#include "calculationProcess.hpp"
#include "stack.hpp"

constexpr long long int MIN = std::numeric_limits< long long int >::min();
constexpr long long int MAX = std::numeric_limits< long long int >::max();

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
    return symbol == "+" || symbol == "-" || symbol == "/" || symbol == "%" || symbol == "*" || symbol == "**";
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

  long long int addition(long long int operand1, long long int operand2)
  {
    if ((operand2 > 0) && (operand1 > (MAX - operand2)))
    {
      throw std::runtime_error("Addition overflow!");
    }
    if ((operand2 < 0) && (operand1 < (MIN - operand2)))
    {
      throw std::runtime_error("Addition underflow!");
    }
    return operand1 + operand2;
  }

  long long int substraction(long long int operand1, long long int operand2)
  {
    if ((operand2 > 0) && (operand1 < (MIN + operand2)))
    {
      throw std::runtime_error("Subtraction underflow!");
    }
    if ((operand2 < 0) && (operand1 > (MAX + operand2)))
    {
      throw std::runtime_error("Subtraction overflow!");
    }
    return operand1 - operand2;
  }

  long long int division(long long int operand1, long long int operand2)
  {
    if (operand2 == 0)
    {
      throw std::runtime_error("Division by zero!");
    }
    if ((operand1 == MIN) && (operand2 == -1))
    {
      throw std::runtime_error("Division overflow!");
    }
    return operand1 / operand2;
  }

  long long int modulation(long long int operand1, long long int operand2)
  {
    if (operand2 == 0)
    {
      throw std::runtime_error("Division by zero!");
    }
    return (operand1 % operand2 + operand2) % operand2;
  }

  long long int multiplication(long long int operand1, long long int operand2)
  {
    if (operand1 > 0)
      {
        if (operand2 > 0)
        {
          if (operand1 > MAX / operand2)
          {
            throw std::runtime_error("Multiplication overflow!");
          }
          else if (operand2 < MIN / operand1)
          {
            throw std::runtime_error("Multiplication underflow!");
          }
        }
      }
      else if (operand1 < 0)
      {
        if (operand2 > 0)
        {
          if (operand1 < MIN / operand2)
          {
            throw std::runtime_error("Multiplication underflow!");
          }
          else if (operand2 != 0 && operand1 > MAX / operand2)
          {
            throw std::runtime_error("Multiplication overflow!");
          }
        }
      }
      return operand1 * operand2;
  }

  long long int power(long long int operand1, long long int operand2)
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
      bool isNegative = (operand1 < 0) && (operand2 % 2 != 0);
      if (operand1 != std::abs(MIN + 1))
      {
        operand1 = std::abs(operand1);
      }
      else
      {
        throw std::runtime_error("Power overflow!");
      }
      long long int result = 1;
      for (long long int i = 0; i < operand2; ++i)
      {
        if (result > MAX / operand1)
        {
          throw std::runtime_error("Power overflow!");
        }
        result *= operand1;
      }
      return isNegative ? -result : result;
  }

  long long int calculateOperation(const std::string& operation, long long int operand1, long long int operand2)
  {
    if (operation == "+")
    {
      return addition(operand1, operand2);
    }
    else if (operation == "-")
    {
      return substraction(operand1, operand2);
    }
    else if (operation == "/")
    {
      return division(operand1, operand2);
    }
    else if (operation == "%")
    {
      return modulation(operand1, operand2);
    }
    else if (operation == "*")
    {
      return multiplication(operand1, operand2);
    }
    else if (operation == "**")
    {
      return power(operand1, operand2);
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
