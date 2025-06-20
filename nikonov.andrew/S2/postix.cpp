#include "postfix.hpp"
#include <iostream>
namespace
{
  bool isOperand(const std::string & el);
  bool isOperator(const std::string & el);
  int getPrecedence(char el);
  bool isLoverPrecedence(const std::string & a, const std::string & b);
  bool isGreaterOrEqualPrecedence(const std::string & a, const std::string & b);
}

nikonov::Postfix::Postfix(const std::string & expr, bool mode)
{
  if (!std::isdigit(expr.back()))
  {
    throw std::logic_error("non-correct infix expression");
  }
  if (mode)
  {
    expression = expr;
    return;
  }
  std::string postfix;
  Stack< std::string > stack;
  size_t cntOfOpenBrackets = 0;
  int cntOfOperands = 0;
  int cntOfOperators = 0;
  for (size_t i = 0; i < expr.size(); ++i)
  {
    std::string tempstr;
    while (expr[i] != ' ' && i < expr.size())
    {
      tempstr += expr[i++];
    }
    if (isOperand(tempstr))
    {
      cntOfOperands++;
      if (!postfix.empty())
      {
        postfix += ' ';
      }
      postfix += tempstr;
    }
    else if ((tempstr.size() == 1) && isOperator(tempstr))
    {
      cntOfOperators++;
      if (stack.empty() || (tempstr.back() == '('))
      {
        if (tempstr.back() == '(')
        {
          ++cntOfOpenBrackets;
        }
        stack.push(tempstr);
      }
      else if (tempstr.back() == ')')
      {
        if (!cntOfOpenBrackets)
        {
          throw std::logic_error("non-correct infix expression, (!cntOfOpenBrackets)");
        }
        while (stack.top().back() != '(')
        {
          postfix += ' ';
          postfix += stack.top();
          stack.pop();
        }
        stack.pop();
        --cntOfOpenBrackets;
      }
      else if (isLoverPrecedence(stack.top(), tempstr))
      {
        stack.push(tempstr);
      }
      else if (isGreaterOrEqualPrecedence(stack.top(), tempstr))
      {
        postfix += ' ';
        postfix += stack.top();
        stack.pop();
        stack.push(tempstr);
      }
    }
    else
    {
      throw std::logic_error("non-correct infix expression");
    }
    if (std::abs((cntOfOperands - cntOfOperators)) > 1)
    {
      throw std::logic_error("non-correct infix expression");
    }
  }
  while (!stack.empty())
  {
    postfix += ' ';
    postfix += stack.top();
    stack.pop();
  }
  expression = postfix;
}
nikonov::Postfix nikonov::Postfix::operator+(const Postfix & rhs)
{
  std::string newexpression = expression + ' ' + rhs.expression + " +";
  return { newexpression, 1 };
}
nikonov::Postfix & nikonov::Postfix::operator+=(const Postfix & rhs)
{
  expression += ' ' + rhs.expression + " +";
  return *this;
}
nikonov::Postfix nikonov::Postfix::operator-(const Postfix & rhs)
{
  std::string newexpression = expression + ' ' + rhs.expression + " -";
  return { newexpression, 1 };
}
nikonov::Postfix & nikonov::Postfix::operator-=(const Postfix & rhs)
{
  expression += ' ' + rhs.expression + " -";
  return *this;
}
nikonov::Postfix nikonov::Postfix::operator*(const Postfix & rhs)
{
  std::string newexpression = expression + ' ' + rhs.expression + " *";
  return { newexpression, 1 };
}
nikonov::Postfix & nikonov::Postfix::operator*=(const Postfix & rhs)
{
  expression += ' ' + rhs.expression + " *";
  return *this;
}
nikonov::Postfix nikonov::Postfix::operator/(const Postfix & rhs)
{
  std::string newexpression = expression + ' ' + rhs.expression + " /";
  return { newexpression, 1 };
}
nikonov::Postfix & nikonov::Postfix::operator/=(const Postfix & rhs)
{
  expression += ' ' + rhs.expression + " /";
  return *this;
}
nikonov::Postfix nikonov::Postfix::operator%(const Postfix & rhs)
{
  std::string newexpression = expression + ' ' + rhs.expression + " %";
  return { newexpression, 1 };
}
nikonov::Postfix & nikonov::Postfix::operator%=(const Postfix & rhs)
{
  expression += ' ' + rhs.expression + " %";
  return *this;
}

namespace
{
  bool isLoverPrecedence(const std::string & a, const std::string & b)
  {
   return !isGreaterOrEqualPrecedence(a, b);
  }
  bool isGreaterOrEqualPrecedence(const std::string & a, const std::string & b)
  {
    if (getPrecedence(a.back()) >= getPrecedence(b.back()))
    {
      return true;
    }
    return false;
  }
  bool isOperand(const std::string & el)
  {
    try
    {
      size_t pos;
      std::stold(el, &pos);
      return (pos == el.size());
    }
    catch (const std::exception & e)
    {
      return false;
    }
  }
  bool isOperator(const std::string & el)
  {
    if (el.size() > 1 && el.empty())
    {
      return false;
    }
    bool flag = false;
    char ops[] = {'-', '+', '*', '/', '%', '(', ')'};
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
}
