#include "calculatorExpression.hpp"
#include <limits>

namespace
{
  bool isNumber(const std::string & element)
  {
    try
    {
      std::stoll(element);
    }
    catch (const std::exception &)
    {
      return false;
    }
    return true;
  }

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

  long long int calculateOperation(long long int op1, long long int op2, const std::string & el)
  {
    const long long int min = std::numeric_limits< long long int >::min();
    const long long int max = std::numeric_limits< long long int >::max();
    if (el == "+")
    {
      if ((op2 > 0) && (op1 > (max - op2)))
      {
        throw std::runtime_error("ERROR: addition overflow");
      }
      if ((op2 < 0) && (op1 < (min - op2)))
      {
        throw std::runtime_error("ERROR: addition underflow");
      }
      return op1 + op2;
    }
    else if (el == "-")
    {
      if ((op2 > 0) && (op1 < (min + op2)))
      {
        throw std::runtime_error("ERROR: subtraction underflow");
      }
      if ((op2 < 0) && (op1 > (max + op2)))
      {
        throw std::runtime_error("ERROR: subtraction overflow");
      }
      return op1 - op2;
    }
    else if (el == "*")
    {
      if ((op2 != 0) && (op1 > (max / op2)))
      {
        throw std::runtime_error("ERROR: multiplication overflow");
      }
      if ((op2 != 0) && (op1 < (min / op2)))
      {
        throw std::runtime_error("ERROR: multiplication underflow");
      }
      return op1 * op2;
    }
    else if (el == "/")
    {
      if (op2 == 0)
      {
        throw std::runtime_error("ERROR: division by zero");
      }
      if ((op1 == min) && (op2 == -1))
      {
        throw std::runtime_error("ERROR: division overflow");
      }
      return op1 / op2;
    }
    else if (el == "%")
    {
      if (op2 == 0)
      {
        throw std::runtime_error("ERROR: division by zero");
      }
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
    if (isNumber(element))
    {
      stack.push(std::stoll(element));
    }
    else if (isOperator(element))
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
    else if (isNumber(element))
    {
      postfixQueue.push(element);
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
      int elPrecedence = getPrecedence(element);
      while (!stack.empty() && elPrecedence <= getPrecedence(stack.top()))
      {
        postfixQueue.push(stack.top());
        stack.pop();
      }
      stack.push(element);
    }
    else
    {
      throw std::runtime_error("ERROR: invalid element");
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
