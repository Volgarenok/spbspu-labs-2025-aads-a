#include "postfix.hpp"
#include <iostream>
namespace
{
  bool isOperand(const std::string & el);
  bool isOperator(const std::string & el);
  int getPrecedence(char el);
  bool isGreaterOrEqualPrecedence(const std::string & a, const std::string & b);
}

nikonov::Postfix::Postfix(const std::string& expr, bool mode)
{
  if (mode)
  {
    expression = expr;
    return;
  }
  std::string postfix;
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
      if (!postfix.empty())
      {
        postfix += ' ';
      }
      postfix += tempstr;
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
        postfix += ' ';
        postfix += stack.top();
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
          postfix += ' ';
          postfix += stack.top();
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
  bool isGreaterOrEqualPrecedence(const std::string & a, const std::string & b)
  {
    return getPrecedence(a.back()) >= getPrecedence(b.back());
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
