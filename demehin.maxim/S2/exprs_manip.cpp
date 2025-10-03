#include "exprs_manip.hpp"
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <calc_utils.hpp>

namespace
{
  using long_t = long long int;

  bool isOperator(const std::string& str)
  {
    return str == "+" || str == "-" || str == "*" || str == "/" || str == "%";
  }

  int getOpPriority(const std::string& op)
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

  int compareOpPriority(const std::string& op1, const std::string& op2)
  {
    return getOpPriority(op1) - getOpPriority(op2);
  }

  long_t performOperation(long_t op1, long_t op2, const std::string& operation)
  {
    if (operation == "+")
    {
      return demehin::sumChecked(op1, op2);
    }
    if (operation == "-")
    {
      return demehin::subtractChecked(op1, op2);
    }
    if (operation == "*")
    {
      return demehin::multipChecked(op1, op2);
    }
    if (operation == "/")
    {
      return demehin::divideChecked(op1, op2);
    }
    if (operation == "%")
    {
      return demehin::modulateChecked(op1, op2);
    }
    throw std::logic_error("invalid operation");
  }
}

demehin::ExprQueue demehin::convertQueue(ExprQueue& infExpr)
{
  ExprQueue postExpr;
  Stack< std::string > stack;
  while (!infExpr.empty())
  {
    std::string front = infExpr.front();
    infExpr.pop();
    try
    {
      std::stoll(front);
      postExpr.push(front);
    }
    catch (...)
    {
      if (front == "(")
      {
        stack.push(front);
      }
      else if (front == ")")
      {
        while (!stack.empty() && stack.top() != "(")
        {
          postExpr.push(stack.top());
          stack.pop();
        }
        if (stack.empty())
        {
          throw std::logic_error("wrong brackets");
        }
        stack.pop();
      }
      else if (isOperator(front))
      {
        while (!stack.empty() && compareOpPriority(front, stack.top()) <= 0)
        {
          postExpr.push(stack.top());
          stack.pop();
        }
        stack.push(front);
      }
      else
      {
        throw std::logic_error("invalid argument");
      }
    }
  }
  while (!stack.empty())
  {
    if (stack.top() == "(")
    {
      throw std::logic_error("wrong brackets");
    }
    postExpr.push(stack.top());
    stack.pop();
  }
  return postExpr;
}

void demehin::convertStack(ExprsStack& infStack, ExprsStack& postStack)
{
  while (!infStack.empty())
  {
    postStack.push(convertQueue(infStack.top()));
    infStack.pop();
  }
}

long_t demehin::calculateExpr(const ExprQueue& expr)
{
  Stack< long_t > stack;
  ExprQueue exprCpy(expr);
  while (!exprCpy.empty())
  {
    std::string token = exprCpy.front();
    exprCpy.pop();

    try
    {
      stack.push(std::stoll(token));
    }
    catch (...)
    {
      if (stack.size() < 2)
      {
        throw std::logic_error("not enough operands");
      }
      long_t op1 = stack.top();
      stack.pop();
      long_t op2 = stack.top();
      stack.pop();

      long_t result = performOperation(op2, op1, token);
      stack.push(result);
    }
  }

  return stack.top();
}

void demehin::getExprsValues(ExprsStack postStack, Stack< long_t >& values)
{
  while (!postStack.empty())
  {
    values.push(calculateExpr(postStack.top()));
    postStack.pop();
  }
}

