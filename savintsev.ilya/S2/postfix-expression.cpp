#include "postfix-expression.h"
#include <stdexcept>
#include <iostream>
#include <limits>
#include "stack.hpp"

namespace
{
  const long long MAX_LL = std::numeric_limits< long long >::max();
  const long long MIN_LL = std::numeric_limits< long long >::min();

  int sign(long long val)
  {
    return (val > 0) ? 1 : ((val < 0) ? -1 : 0);
  }

  bool same_sign(long long a, long long b)
  {
    return sign(a) * sign(b) > 0;
  }

  long long sum(long long a, long long b)
  {
    if (same_sign(a, b) && (a > 0))
    {
      if (MAX_LL - a >= b)
      {
        return a + b;
      }
    }
    else if (same_sign(a, b) && (a < 0))
    {
      if (MIN_LL - a <= b)
      {
        return a + b;
      }
    }
    else if (!same_sign(a, b))
    {
      return a + b;
    }
    throw std::overflow_error("ERROR: Addition overflow");
  }

  long long diff(long long a, long long b)
  {
    if (same_sign(a, b))
    {
      return a - b;
    }
    else if (!same_sign(a, b) && a <= 0)
    {
      if (MIN_LL - a <= b)
      {
        return a - b;
      }
    }
    else if (!same_sign(a, b) && a >= 0)
    {
      if (MAX_LL - a >= b)
      {
        return a - b;
      }
    }
    throw std::overflow_error("ERROR: Subtraction overflow");
  }

  long long prod(long long a, long long b)
  {
    if (a == 0 || b == 0)
    {
      return 0ll;
    }
    else if (same_sign(a, b) && (a > 0))
    {
      if (MAX_LL / a >= b)
      {
        return a * b;
      }
    }
    else if (same_sign(a, b) && (a < 0))
    {
      if (MIN_LL / a <= b)
      {
        return a * b;
      }
    }
    else if (!same_sign(a, b))
    {
      if (std::abs(MIN_LL / a) >= std::abs(b))
      {
        return a * b;
      }
    }
    throw std::overflow_error("ERROR: Multiplication overflow");
  }

  long long quot(long long a, long long b)
  {
    if (b == 0)
    {
      throw std::invalid_argument("ERROR: Division by zero");
    }
    else if (a == 0)
    {
      return 0ll;
    }
    else if (same_sign(a, b) && (a < 0))
    {
      if (a != MIN_LL && b != -1ll)
      {
        return a / b;
      }
    }
    else
    {
      return a / b;
    }
    throw std::overflow_error("ERROR: Division overflow");
  }

  long long rem(long long a, long long b)
  {
    return (a % b + b) % b;
  }
}

savintsev::PostfixExpr savintsev::convert(const std::string & infix)
{
  PostfixExpr postfix;
  using str = std::string;

  Stack< str > stack;

  size_t was_bracket = 0;

  size_t back = infix.size();
  size_t begin = 0;
  size_t end = 0;

  while (end != back)
  {
    end = infix.find(' ', begin);
    str token = infix.substr(begin, end != str::npos ? end - begin : end = back);
    begin = end + 1;
    if (token.empty())
    {
      continue;
    }
    else if (token == "(")
    {
      stack.push(token);
      was_bracket++;
    }
    else if (token == "+" || token == "-")
    {
      if (!stack.empty())
      {
        if (stack.top() != "(")
        {
          postfix.expr_.push(stack.top());
          stack.pop();
        }
      }
      stack.push(token);
    }
    else if (token == "*" || token == "/" || token == "%")
    {
      if (!stack.empty())
      {
        if (stack.top() == "*" || stack.top() == "*" || stack.top() == "%")
        {
          postfix.expr_.push(stack.top());
          stack.pop();
        }
      }
      stack.push(token);
    }
    else if (token == ")")
    {
      if (!was_bracket)
      {
        throw std::invalid_argument("ERROR: invalid expression");
      }
      while (stack.top() != "(")
      {
        postfix.expr_.push(stack.top());
        stack.pop();
      }
      stack.pop();
      --was_bracket;
    }
    else
    {
      postfix.expr_.push(token);
    }
  }
  while (!stack.empty())
  {
    postfix.expr_.push(stack.top());
    stack.pop();
  }
  if (was_bracket)
  {
    throw std::invalid_argument("ERROR: invalid expression");
  }
  postfix();
  return postfix;
}

long long savintsev::PostfixExpr::operator()() const
{
  Queue< std::string > expr(expr_);
  Stack< long long > calc;

  while (expr.size())
  {
    std::string token = expr.front();
    expr.pop();
    try
    {
      long long num = std::stoll(token);
      calc.push(num);
    }
    catch (const std::invalid_argument & e)
    {
      if (calc.size() < 2)
      {
        throw std::invalid_argument("ERROR: invalid expression");
      }
      long long r = calc.top();
      calc.pop();
      long long l = calc.top();
      calc.pop();
      long long result = 0;
      switch (*(token.data()))
      {
      case '+':
        result = sum(l, r);
        calc.push(result);
        break;
      case '-':
        result = diff(l, r);
        calc.push(result);
        break;
      case '*':
        result = prod(l, r);
        calc.push(result);
        break;
      case '/':
        result = quot(l, r);
        calc.push(result);
        break;
      case '%':
        result = rem(l, r);
        calc.push(result);
        break;
      }
    }
  }
  if (calc.size() == 1)
  {
    return calc.top();
  }
  throw std::invalid_argument("ERROR: invalid expression");
}

savintsev::PostfixExpr & savintsev::PostfixExpr::use_operator(const std::string & op, PostfixExpr rhs)
{
  while (rhs.expr_.size())
  {
    this->expr_.push(rhs.expr_.front());
    rhs.expr_.pop();
  }
  this->expr_.push(op);
  return *this;
}

savintsev::PostfixExpr savintsev::PostfixExpr::operator+(const PostfixExpr & rhs)
{
  PostfixExpr new_lhs(*this);
  return new_lhs.use_operator("+", rhs);
}

savintsev::PostfixExpr savintsev::PostfixExpr::operator-(const PostfixExpr & rhs)
{
  PostfixExpr new_lhs(*this);
  return new_lhs.use_operator("-", rhs);
}

savintsev::PostfixExpr savintsev::PostfixExpr::operator*(const PostfixExpr & rhs)
{
  PostfixExpr new_lhs(*this);
  return new_lhs.use_operator("*", rhs);
}

savintsev::PostfixExpr savintsev::PostfixExpr::operator/(const PostfixExpr & rhs)
{
  PostfixExpr new_lhs(*this);
  return new_lhs.use_operator("/", rhs);
}

savintsev::PostfixExpr savintsev::PostfixExpr::operator%(const PostfixExpr & rhs)
{
  PostfixExpr new_lhs(*this);
  return new_lhs.use_operator("%", rhs);
}
