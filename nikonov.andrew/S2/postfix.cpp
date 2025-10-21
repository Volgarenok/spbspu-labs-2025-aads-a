#include "postfix.hpp"
#include <iostream>
#include <sstream>
#include <limits>
namespace
{
  constexpr long long max_ll = std::numeric_limits< long long >::max();
  constexpr long long min_ll = std::numeric_limits< long long >::min();

  bool isOperand(const std::string & el);
  bool isOperator(const std::string & el);
  int getPrecedence(char el);
  bool isGreaterOrEqualPrecedence(const std::string & a, const std::string & b);

  long long doMath(long long a, long long b, const std::string & op);
  bool isOverflowAddition(long long int a, long long int b);
  bool isOverflowSubstraction(long long int a, long long int b);
  bool isOverflowMulti(long long int a, long long int b);
}

nikonov::Postfix::Postfix(const std::string& expr):
  tokens_()
{
  Stack< std::string > stack;
  size_t cntOfOpenBrackets = 0;
  bool lastWasOperand = false;
  for (size_t i = 0; i < expr.size(); ++i)
  {
    std::string tempstr;
    while (i < expr.size() && expr[i] != ' ')
    {
      tempstr += expr[i++];
    }
    if (isOperand(tempstr))
    {
      if (lastWasOperand)
      {
        throw std::logic_error("non-correct infix expression: two operands in a row");
      }
      tokens_.push(tempstr);
      lastWasOperand = true;
    }
    else if (tempstr == "(")
    {
      cntOfOpenBrackets++;
      stack.push(tempstr);
    }
    else if (tempstr == ")")
    {
      if (!cntOfOpenBrackets)
      {
        throw std::logic_error("non-correct infix expression: mismatched brackets");
      }
      while (!stack.empty() && stack.top() != "(")
      {
        tokens_.push(stack.top());
        stack.pop();
      }
      stack.pop();
      cntOfOpenBrackets--;
    }
    else if (isOperator(tempstr) && lastWasOperand)
    {
      while (!stack.empty() && stack.top() != "(")
      {
        if (isGreaterOrEqualPrecedence(stack.top(), tempstr))
        {
          tokens_.push(stack.top());
          stack.pop();
        }
        else
        {
          break;
        }
      }
      stack.push(tempstr);
      lastWasOperand = false;
    }
    else
    {
      throw std::logic_error("non-correct infix expression");
    }
  }
  while (!stack.empty())
  {
    if (stack.top() == "(")
    {
      throw std::logic_error("non-correct infix expression: mismatched brackets");
    }
    tokens_.push(stack.top());
    stack.pop();
  }
}
nikonov::Postfix nikonov::Postfix::operator+(const Postfix & rhs)
{
  Postfix copyRes(*this);
  return copyRes.doMathOperator(rhs, "+");
}
nikonov::Postfix & nikonov::Postfix::operator+=(const Postfix & rhs)
{
  doMathOperator(rhs, "+");
  return *this;
}
nikonov::Postfix nikonov::Postfix::operator-(const Postfix & rhs)
{
  Postfix copyRes(*this);
  return copyRes.doMathOperator(rhs, "-");
}
nikonov::Postfix & nikonov::Postfix::operator-=(const Postfix & rhs)
{
  doMathOperator(rhs, "-");
  return *this;
}
nikonov::Postfix nikonov::Postfix::operator*(const Postfix & rhs)
{
  Postfix copyRes(*this);
  return copyRes.doMathOperator(rhs, "*");
}
nikonov::Postfix & nikonov::Postfix::operator*=(const Postfix & rhs)
{
  doMathOperator(rhs, "*");
  return *this;
}
nikonov::Postfix nikonov::Postfix::operator/(const Postfix & rhs)
{
  Postfix copyRes(*this);
  return copyRes.doMathOperator(rhs, "/");
}
nikonov::Postfix & nikonov::Postfix::operator/=(const Postfix & rhs)
{
  doMathOperator(rhs, "/");
  return *this;
}
nikonov::Postfix nikonov::Postfix::operator%(const Postfix & rhs)
{
  Postfix copyRes(*this);
  return copyRes.doMathOperator(rhs, "%");
}
nikonov::Postfix & nikonov::Postfix::operator%=(const Postfix & rhs)
{
  doMathOperator(rhs, "%");
  return *this;
}
long long nikonov::Postfix::operator()() const
{
  Stack< long long > postfixOperands;
  Queue< std::string > tempPostfixTokens(tokens_);
  long long operand1 = 0;
  long long operand2 = 0;
  long long result = 0;
  while (!tempPostfixTokens.empty())
  {
    if (isOperand(tempPostfixTokens.front()))
    {
      postfixOperands.push(std::stoll(tempPostfixTokens.front()));
      tempPostfixTokens.pop();
    }
    else if (isOperator(tempPostfixTokens.front()))
    {
      if (postfixOperands.size() < 2)
      {
        throw std::invalid_argument("Incorrect expression");
      }
      operand2 = postfixOperands.top();
      postfixOperands.pop();
      operand1 = postfixOperands.top();
      postfixOperands.pop();
      result = doMath(operand1, operand2, tempPostfixTokens.front());
      postfixOperands.push(result);
      tempPostfixTokens.pop();
    }
  }
  if (postfixOperands.size() != 1)
  {
    throw std::invalid_argument("Incorrect expression");
  }
  return postfixOperands.top();
}
nikonov::Postfix& nikonov::Postfix::doMathOperator(const Postfix& other, const std::string& operation)
{
  Postfix copy(other);
  while (!copy.tokens_.empty()) {
    tokens_.push(copy.tokens_.front());
    copy.tokens_.pop();
  }
  tokens_.push(operation);
  return *this;
}

namespace
{
  bool isGreaterOrEqualPrecedence(const std::string & a, const std::string & b)
  {
    return getPrecedence(a.back()) >= getPrecedence(b.back());
  }
  bool isOperand(const std::string & el)
  {
    try
    {
      size_t pos;
      std::stoll(el, &pos);
      return (pos == el.size());
    }
    catch (const std::exception & e)
    {
      return false;
    }
  }
  bool isOperator(const std::string & el)
  {
    if (el.size() > 1 || el.empty())
    {
      return false;
    }
    bool flag = false;
    char ops[] = {'-', '+', '*', '/', '%'};
    for (auto op : ops)
    {
      if (el.back() == op)
      {
        flag = true;
      }
    }
    return flag;
  }
  int getPrecedence(char el)
  {
    if (el == '+' || el == '-')
    {
      return 2;
    }
    else if (el == '*'|| el == '/'|| el == '%')
    {
      return 3;
    }
    else if (el == '(' || el == ')')
    {
      return 0;
    }
    throw std::logic_error("ERROR: this op doesn't supported");
  }
  long long doMath(long long a, long long b, const std::string & op)
  {
    switch (op.front())
    {
      case ('+'):
      {
        if (isOverflowAddition(a, b))
        {
          throw std::overflow_error("Addition overflow");
        }
        return a + b;
      }
      case ('-'):
      {
        if (isOverflowSubstraction(a, b))
        {
          throw std::overflow_error("Substraction overflow");
        }
        return a - b;
      }
      case ('*'):
      {
        if (isOverflowMulti(a, b))
        {
          throw std::overflow_error("Multiplication overflow");
        }
        return a * b;
      }
      case ('/'):
      {
        if (b == 0)
        {
          throw std::logic_error("divided by zero");
        }
        return a / b;
      }
      case ('%'):
      {
        if (b == 0)
        {
          throw std::logic_error("modulo by zero");
        }
        return (a % b + b) % b;
      }
    }
    throw std::logic_error("unknown operator");
  }
  bool isOverflowAddition(long long int lhs, long long int rhs)
  {
    if (lhs > 0 && rhs > 0)
    {
      return ((max_ll - rhs) < lhs);
    }
    if (lhs < 0 && rhs < 0)
    {
      return ((min_ll - rhs) > lhs);
    }
    return false;
  }
  bool isOverflowSubstraction(long long int lhs, long long int rhs)
  {
    if (lhs > 0 && rhs < 0)
    {
      return ((max_ll + rhs) < lhs);
    }
    if (lhs < 0 && rhs > 0)
    {
      return ((min_ll + rhs) > lhs);
    }
    return false;
  }
  bool isOverflowMulti(long long int lhs, long long int rhs)
  {
    if ((lhs > 0 && rhs > 0) || (lhs < 0 && rhs < 0))
    {
      return (std::abs(max_ll / lhs) < std::abs(rhs));
    }
    else
    {
      return (std::abs(min_ll / lhs) < std::abs(rhs));
    }
    return false;
  }
}
