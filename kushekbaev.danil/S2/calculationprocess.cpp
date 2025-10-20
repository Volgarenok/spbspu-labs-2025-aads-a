#include "calculationprocess.hpp"
#include <limits>
#include "stack.hpp"

namespace
{
  constexpr long long int MIN = std::numeric_limits< long long int >::min();
  constexpr long long int MAX = std::numeric_limits< long long int >::max();

  bool isOperator(const std::string& symbol)
  {
    const char* ops[] = {"+", "-", "*", "/", "%", "**"};
    for (const char* op: ops)
    {
      if (symbol == op)
      {
        return true;
      }
    }
    return false;
  }

  int calculatePriority(const std::string& operation)
  {
    const std::pair< const char*, int > prios[] =
    {
      { "+", 1 }, { "-", 1 }, { "*", 2 }, { "/", 2 }, { "%", 2 }, { "**", 3 }
    };
    for (const auto& op: prios)
    {
      if (operation == op.first)
      {
        return op.second;
      }
    }
    return 0;
  }

  long long int add(long long int operand1, long long int operand2)
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

  long long int substract(long long int operand1, long long int operand2)
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

  long long int divide(long long int operand1, long long int operand2)
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

  long long int modulate(long long int operand1, long long int operand2)
  {
    if (operand2 == 0)
    {
      throw std::runtime_error("Division by zero!");
    }
    return (operand1 % operand2 + operand2) % operand2;
  }

  long long int multiplicate(long long int operand1, long long int operand2)
  {
    if (operand1 == 0 || operand2 == 0)
    {
      return 0;
    }

    if (operand1 > 0 && operand2 > 0)
    {
      if (operand1 > MAX / operand2)
      {
        throw std::runtime_error("Multiplication overflow!");
      }
      return operand1 * operand2;
    }

    if (operand1 > 0 && operand2 < 0)
    {
      if (operand1 > MIN / operand2)
      {
        throw std::runtime_error("Multiplication overflow!");
      }
      if (operand2 > MAX / operand1)
      {
        throw std::runtime_error("Multiplication underflow!");
      }
      return operand1 * operand2;
    }

    if (operand1 < 0 && operand2 > 0)
    {
      if (operand1 < MIN / operand2)
      {
        throw std::runtime_error("Multiplication underflow!");
      }
      if (operand1 > MAX / operand2)
      {
        throw std::runtime_error("Multiplication overflow!");
      }
      return operand1 * operand2;
    }

    if (operand1 < MAX / operand2)
    {
      throw std::runtime_error("Multiplication underflow!");
    }

    if (operand1 > MIN / operand2)
    {
      throw std::runtime_error("Multiplication overflow!");
    }
    return operand1 * operand2;
  }

  long long int power(long long int x, long long int power)
  {
    if (x == 0 && power > 0)
    {
      return 0;
    }
    if (x == 0)
    {
      throw std::runtime_error("Undefined power!");
    }
    if (power == 0)
    {
      return 1;
    }
    bool isNegative = (x < 0) && (power % 2 != 0);
    if (x < MAX - 1)
    {
      x = std::abs(x);
    }
    else
    {
      throw std::runtime_error("Power overflow!");
    }
    long long int result = 1;
    for (long long int i = 0; i < power; ++i)
    {
      if (result > MAX / x)
      {
        throw std::runtime_error("Power overflow!");
      }
      result = multiplicate(result, x);
    }
    return isNegative ? -result : result;
  }

  long long int calculateOperation(const std::string& operation, long long int operand1, long long int operand2)
  {
    if (operation == "+")
    {
      return add(operand1, operand2);
    }
    else if (operation == "-")
    {
      return substract(operand1, operand2);
    }
    else if (operation == "/")
    {
      return divide(operand1, operand2);
    }
    else if (operation == "%")
    {
      return modulate(operand1, operand2);
    }
    else if (operation == "*")
    {
      return multiplicate(operand1, operand2);
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

kushekbaev::Queue< std::string > kushekbaev::convertToPostfix(Queue< std::string >& queue)
{
  Queue< std::string > postfixQueue;
  Stack< std::string > stack;
  std::string symbol;
  while (!queue.empty())
  {
    symbol = queue.front();
    if (symbol == "(")
    {
      stack.push(symbol);
    }
    else if (isOperator(symbol))
    {
      int priority = calculatePriority(symbol);
      while (!stack.empty() && priority <= calculatePriority(stack.top()))
      {
        postfixQueue.push(stack.top());
        stack.pop();
      }
      stack.push(symbol);
    }
    else if (symbol == ")")
    {
      while (!stack.empty() && stack.top() != "(")
      {
        postfixQueue.push(stack.top());
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
      postfixQueue.push(symbol);
    }
    queue.pop();
  }
  while (!stack.empty())
  {
    postfixQueue.push(stack.top());
    stack.pop();
  }
  return postfixQueue;
}

long long int kushekbaev::calculatePostfix(Queue< std::string > postfixQueue)
{
  Stack< long long int > stack;
  while (!postfixQueue.empty())
  {
    std::string symbol = postfixQueue.front();
    postfixQueue.pop();
    if (isOperator(symbol))
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
    else
    {
      try
      {
        stack.push(std::stoll(symbol));
      }
      catch (const std::exception&)
      {
        throw std::runtime_error("Invalid number in postfix expression");
      }
    }
  }
  if (stack.size() != 1)
  {
    throw std::runtime_error("Invalid expression: too many operands");
  }
  long long int result = stack.top();
  stack.pop();
  return result;
}
