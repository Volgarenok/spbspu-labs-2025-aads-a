#include "calculatorExpression.hpp"
#include <limits>

namespace
{
  bool isOperator(const std::string & el)
  {
    return (el == "+" || el == "-" || el == "*" || el == "/" || el == "%");
  }

  int getPrecedence(const std::string & op)
  {
    if (op == "+" || op == "-")
    {
      return 1;
    }
    if (op == "*" || op == "/" || op == "%")
    {
      return 2;
    }
    return 0;
  }

  bool isHighPrecedence(const std::string & op1, const std::string & op2)
  {
    return getPrecedence(op1) >= getPrecedence(op2);
  }

  void checkAddOverflow(long long int op1, long long int op2)
  {
    const long long int max = std::numeric_limits< long long int >::max();
    const long long int min = std::numeric_limits< long long int >::min();
    if ((op2 > 0) && (op1 > max - op2))
    {
      throw std::runtime_error("ERROR: addition overflow");
    }
    if ((op2 < 0) && (op1 < min - op2))
    {
      throw std::runtime_error("ERROR: addition underflow");
    }
  }

  void checkSubOverflow(long long int op1, long long int op2)
  {
    const long long int max = std::numeric_limits< long long int >::max();
    const long long int min = std::numeric_limits< long long int >::min();
    if ((op2 > 0) && (op1 < min + op2))
    {
      throw std::runtime_error("ERROR: subtraction underflow");
    }
    if ((op2 < 0) && (op1 > max + op2))
    {
      throw std::runtime_error("ERROR: subtraction overflow");
    }
  }

  void checkMulOverflow(long long int op1, long long int op2)
  {
    const long long int max = std::numeric_limits< long long int >::max();
    const long long int min = std::numeric_limits< long long int >::min();
    if (op2 != 0)
    {
      if (op1 > max / op2)
      {
        throw std::runtime_error("ERROR: multiplication overflow");
      }
      if (op1 < min / op2)
      {
        throw std::runtime_error("ERROR: multiplication underflow");
      }
    }
  }

  void checkDivOverflow(long long int op1, long long int op2)
  {
    const long long int min = std::numeric_limits< long long int >::min();
    if (op2 == 0)
    {
      throw std::runtime_error("ERROR: division by zero");
    }
    if (op1 == min && op2 == -1)
    {
      throw std::runtime_error("ERROR: division overflow");
    }
  }

  void checkModByZero(long long int op2)
  {
    if (op2 == 0)
    {
      throw std::runtime_error("ERROR: division by zero");
    }
  }

  long long int calculateOperation(long long int op1, long long int op2, const std::string & el)
  {
    if (el == "+")
    {
      checkAddOverflow(op1, op2);
      return op1 + op2;
    }
    else if (el == "-")
    {
      checkSubOverflow(op1, op2);
      return op1 - op2;
    }
    else if (el == "*")
    {
      checkMulOverflow(op1, op2);
      return op1 * op2;
    }
    else if (el == "/")
    {
      checkDivOverflow(op1, op2);
      return op1 / op2;
    }
    else if (el == "%")
    {
      checkModByZero(op2);
      return (op1 % op2 + op2) % op2;
    }
    else
    {
      throw std::runtime_error("ERROR: invalid operator");
    }
  }
}

long long int maslov::calculatePostfix(Queue< std::string > postfixQueue)
{
  Stack< long long int > stack;
  while (!postfixQueue.empty())
  {
    std::string element = postfixQueue.front();
    postfixQueue.pop();
    if (!isOperator(element))
    {
      try
      {
        stack.push(std::stoll(element));
      }
      catch (const std::exception &)
      {
        throw std::runtime_error("ERROR: invalid number\n");
      }
    }
    else
    {
      if (stack.size() < 2)
      {
        throw std::runtime_error("ERROR: the operator requires at least 2 operands");
      }
      long long int operand2 = stack.top();
      stack.pop();
      long long int operand1 = stack.top();
      stack.pop();
      stack.push(calculateOperation(operand1, operand2, element));
    }
  }
  long long int result  = stack.top();
  stack.pop();
  return result;
}

maslov::Queue< std::string > maslov::infixToPostfix(Queue< std::string > infixQueue)
{
  Queue< std::string > postfixQueue;
  Stack< std::string > stack;
  while (!infixQueue.empty())
  {
    std::string element = infixQueue.front();
    if (element == "(")
    {
      stack.push(element);
    }
    else if (element == ")")
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
        throw std::runtime_error("ERROR: not found parentheses");
      }
    }
    else if (isOperator(element))
    {
      while (!stack.empty() && isHighPrecedence(stack.top(), element))
      {
        postfixQueue.push(stack.top());
        stack.pop();
      }
      stack.push(element);
    }
    else
    {
      postfixQueue.push(element);
    }
    infixQueue.pop();
  }
  while (!stack.empty())
  {
    postfixQueue.push(stack.top());
    stack.pop();
  }
  return postfixQueue;
}
