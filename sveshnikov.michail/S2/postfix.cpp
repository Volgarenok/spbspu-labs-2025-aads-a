#include "postfix.hpp"
#include <stdexcept>
#include <limits>

namespace
{
  constexpr long long max_ll = std::numeric_limits< long long >::max();
  constexpr long long min_ll = std::numeric_limits< long long >::min();

  bool isOperator(const std::string &op);
  size_t getPriority(const std::string &op);
  bool comparePriorities(const std::string &op1, const std::string &op2);

  void isDivisionByZero(long long operand);

  long long add(long long res, long long op);
  long long subtract(long long res, long long op);
  long long multiply(long long res, long long op);
  long long divide(long long res, long long op);
  long long moduloDivide(long long res, long long op);
  long long power(long long res, long long exp);
  long long doublePower(long long res, long long op);

  bool isOperator(const std::string &op)
  {
    return (op == "+" || op == "-" || op == "*" || op == "%" || op == "/" || op == "^^");
  }

  size_t getPriority(const std::string &op)
  {
    if (op == "(")
    {
      return 0;
    }
    if (op == ")")
    {
      return 1;
    }
    if (op == "+" || op == "-")
    {
      return 2;
    }
    if (op == "*" || op == "/" || op == "%")
    {
      return 3;
    }
    if (op == "^^")
    {
      return 4;
    }
    throw std::invalid_argument("ERROR: Unknown operator!");
  }

  bool comparePriorities(const std::string &op1, const std::string &op2)
  {
    return getPriority(op1) <= getPriority(op2);
  }

  void isDivisionByZero(long long operand)
  {
    if (operand == 0)
    {
      throw std::overflow_error("ERROR: Division by zero!");
    }
  }

  long long add(long long res, long long op)
  {
    if (op > 0 && res > max_ll - op)
    {
      throw std::overflow_error("ERROR: Overflow when calculating the amount!");
    }
    else if (op < 0 && res < min_ll - op)
    {
      throw std::underflow_error("ERROR: Underflow when calculating the amount!");
    }
    return res + op;
  }

  long long subtract(long long res, long long op)
  {
    if (op > 0 && res < min_ll + op)
    {
      throw std::underflow_error("ERROR: Underflow when calculating the difference!");
    }
    else if (op < 0 && res > max_ll + op)
    {
      throw std::overflow_error("ERROR: Overflow when calculating the difference!");
    }
    return res - op;
  }

  long long multiply(long long res, long long op)
  {
    if ((op == -1 && res == min_ll) || (op == min_ll && res == -1))
    {
      throw std::overflow_error("ERROR: Overflow when calculating the product!");
    }
    if (res != 0)
    {
      if ((op > 0 && res > max_ll / op) || (op < 0 && res < max_ll / op))
      {
        throw std::overflow_error("ERROR: Overflow when calculating the product!");
      }
      if ((op > 0 && res < min_ll / op) || (op < 0 && res > min_ll / op))
      {
        throw std::underflow_error("ERROR: Underflow when calculating the product!");
      }
    }
    return res * op;
  }

  long long divide(long long res, long long op)
  {
    isDivisionByZero(op);
    return res / op;
  }

  long long moduloDivide(long long res, long long op)
  {
    isDivisionByZero(op);
    res %= op;
    if (res < 0)
    {
      res += std::abs(op);
    }
    return res;
  }

  long long power(long long res, long long exp)
  {
    if (exp < 0)
    {
      throw std::logic_error("Error: negative degree!");
    }
    if (res == 0 && exp == 0)
    {
      throw std::logic_error("Error: Raising zero to zero degree!");
    }
    if (exp == 0)
    {
      return 1;
    }
    long long base = res;
    for (long long i = 1; i < exp; i++)
    {
      res = multiply(res, base);
    }
    return res;
  }

  long long doublePower(long long res, long long op)
  {
    op = power(op, op);
    return power(res, op);
  }
}

sveshnikov::Postfix::Postfix(const Postfix &other):
  expr_(other.expr_)
{}

sveshnikov::Postfix::Postfix(Postfix &&other):
  expr_(std::move(other.expr_))
{}

sveshnikov::Postfix::Postfix(Queue< std::string > infix):
  expr_()
{
  Stack< std::string > stack;
  size_t num_open_paren = 0, num_close_paren = 0;
  size_t num_operators = 0, num_operands = 0;
  if (infix.empty())
  {
    return;
  }
  while (!infix.empty())
  {
    if (infix.front() == "(")
    {
      if (!stack.empty() && !isOperator(stack.top()) && stack.top() != "(")
      {
        throw std::logic_error("ERROR: Incorrect infix notation!");
      }
      stack.push("(");
      num_open_paren++;
    }
    else if (infix.front() == ")")
    {
      num_close_paren++;
      if (num_close_paren > num_open_paren || num_operands != num_operators + 1)
      {
        throw std::logic_error("ERROR: Incorrect infix notation!");
      }
      push_out_stack(stack, infix.front());
      stack.pop();
    }
    else if (isOperator(infix.front()))
    {
      num_operators++;
      if (num_operators > num_operands)
      {
        throw std::logic_error("ERROR: Incorrect infix notation!");
      }
      push_out_stack(stack, infix.front());
      stack.push(infix.front());
    }
    else
    {
      num_operands++;
      if (num_operands > num_operators + 1)
      {
        throw std::logic_error("ERROR: Incorrect infix notation!");
      }
      stoll(infix.front());
      expr_.push(infix.front());
    }
    infix.pop();
  }
  push_out_stack(stack, ")");
  if (num_close_paren != num_open_paren || num_operands != num_operators + 1)
  {
    throw std::logic_error("ERROR: Incorrect infix notation!");
  }
}

void sveshnikov::Postfix::push_out_stack(Stack< std::string > &stack, const std::string &op)
{
  while (!stack.empty() && comparePriorities(op, stack.top()))
  {
    expr_.push(stack.top());
    stack.pop();
  }
}

sveshnikov::Postfix &sveshnikov::Postfix::operator=(const Postfix &other)
{
  expr_ = other.expr_;
  return *this;
}

sveshnikov::Postfix &sveshnikov::Postfix::operator=(Postfix &&other) noexcept
{
  expr_ = std::move(other.expr_);
  return *this;
}

sveshnikov::Postfix sveshnikov::Postfix::operator+(const Postfix &other) const
{
  return arith_operator_impl(other, "+");
}

sveshnikov::Postfix sveshnikov::Postfix::operator-(const Postfix &other) const
{
  return arith_operator_impl(other, "-");
}

sveshnikov::Postfix sveshnikov::Postfix::operator/(const Postfix &other) const
{
  return arith_operator_impl(other, "/");
}

sveshnikov::Postfix sveshnikov::Postfix::operator%(const Postfix &other) const
{
  return arith_operator_impl(other, "%");
}

sveshnikov::Postfix sveshnikov::Postfix::operator*(const Postfix &other) const
{
  return arith_operator_impl(other, "*");
}

sveshnikov::Postfix sveshnikov::Postfix::arith_operator_impl(Postfix other,
    const std::string &op) const
{
  Postfix new_postfix(*this);
  while (!other.expr_.empty())
  {
    new_postfix.expr_.push(other.expr_.front());
    other.expr_.pop();
  }
  new_postfix.expr_.push(op);
  return new_postfix;
}

long long sveshnikov::Postfix::calculate() const
{
  Stack< long long > stack;
  Queue< std::string > queue(expr_);
  long long res = 0;
  while (!queue.empty())
  {
    std::string curr = std::move(queue.front());
    queue.pop();
    if (isOperator(curr))
    {
      long long op = stack.top();
      stack.pop();
      res = stack.top();
      stack.pop();
      if (curr == "+")
      {
        res = add(res, op);
      }
      if (curr == "-")
      {
        res = subtract(res, op);
      }
      if (curr == "/")
      {
        res = divide(res, op);
      }
      if (curr == "%")
      {
        res = moduloDivide(res, op);
      }
      if (curr == "*")
      {
        res = multiply(res, op);
      }
      if (curr == "^^")
      {
        res = doublePower(res, op);
      }
      stack.push(res);
    }
    else
    {
      stack.push(std::stoll(curr));
    }
    res = stack.top();
  }
  return res;
}
