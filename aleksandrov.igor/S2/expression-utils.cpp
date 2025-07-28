#include "expression-utils.hpp"
#include <iostream>
#include <sstream>
#include <limits>
#include "expression-part.hpp"

namespace
{
  using namespace aleksandrov;

  const long long int max = std::numeric_limits< long long int >::max();
  const long long int min = std::numeric_limits< long long int >::min();

  bool isLessImportant(const ExpressionPart& a, const ExpressionPart& b)
  {
    using Op = OperationType;
    Op first = a.getOperation();
    Op second = b.getOperation();

    if (first == Op::Addition || first == Op::Subtraction)
    {
      return second == Op::Multiplication || second == Op::Division || second == Op::Modulo;
    }
    return false;
  }

  short int sign(OperandType val)
  {
    return (val > 0) ? 1 : ((val < 0) ? -1 : 0);
  }

  bool sameSign(OperandType a, OperandType b)
  {
    return sign(a) * sign(b) > 0;
  }

  OperandType sum(OperandType a, OperandType b)
  {
    if (!(a || b) || a == -b)
    {
      return 0;
    }
    if (sameSign(a, b) && ((a > 0 && max - a < b) || (a < 0 && min - a > b)))
    {
      throw std::overflow_error("Overflow error!");
    }
    return a + b;
  }

  OperandType multiply(OperandType a, OperandType b)
  {
    if (!a || !b)
    {
      return 0;
    }
    bool isSameSignOverflow = (b > 0 && (a > max / b || a < min / b));
    bool isDiffSignOverflow = (b < 0 && (a < min / b || a > max / b));
    if (isSameSignOverflow || isDiffSignOverflow)
    {
      throw std::overflow_error("Overflow error!");
    }
    return a * b;
  }

  OperandType divide(OperandType a, OperandType b)
  {
    if (!b)
    {
      throw std::invalid_argument("Division by zero!");
    }
    if (a == min && b == -1)
    {
      throw std::overflow_error("Overflow error!");
    }
    if (std::abs(a) < std::abs(b))
    {
      return 0;
    }
    return a / b;
  }

  OperandType modulo(OperandType a, OperandType b)
  {
    if (!b)
    {
      throw std::invalid_argument("Division by zero!");
    }
    return (a % b + std::abs(b)) % std::abs(b);
  }
}

std::istream& aleksandrov::operator>>(std::istream& in, ExpressionPart& token)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }

  OperandType num = 0;
  if (std::isdigit(in.peek()))
  {
    in >> num;
    token = ExpressionPart(num);
    return in;
  }

  in.clear();
  char c = '\0';
  if (in >> c)
  {
    try
    {
      token = ExpressionPart(c);
    }
    catch (const std::logic_error&)
    {
      in.setstate(std::ios::failbit);
    }
  }
  return in;
}

void aleksandrov::getExpression(std::istream& in, Expression& expr)
{
  ExpressionPart token('+');
  while (in >> token)
  {
    expr.push(token);
  }
}

void aleksandrov::getExpressions(std::istream& in, Expressions& exprs)
{
  std::string line;
  while (std::getline(in, line))
  {
    if (!line.empty())
    {
      std::istringstream iss(line);
      Expression expr;
      getExpression(iss, expr);
      exprs.push(expr);
    }
  }
}

aleksandrov::Expression aleksandrov::getPostfixForm(Expression& expr)
{
  Expression postfix;
  Stack< ExpressionPart > stack;

  while (!expr.empty())
  {
    ExpressionPart token(expr.front());
    expr.pop();

    if (token.isOperand())
    {
      postfix.push(token);
    }
    else if (token.isOpeningBracket())
    {
      stack.push(token);
    }
    else if (token.isClosingBracket())
    {
      while (!stack.empty() && !stack.top().isOpeningBracket())
      {
        postfix.push(stack.top());
        stack.pop();
      }
      if (stack.empty())
      {
        throw std::logic_error("Mismatched brackets!");
      }
      stack.pop();
    }
    else if (token.isOperation())
    {
      while (!stack.empty() && stack.top().isOperation() && !isLessImportant(stack.top(), token))
      {
        postfix.push(stack.top());
        stack.pop();
      }
      stack.push(token);
    }
    else
    {
      throw std::logic_error("Incorrect expression!");
    }
  }

  while (!stack.empty())
  {
    if (stack.top().isOpeningBracket() || stack.top().isClosingBracket())
    {
      throw std::logic_error("Incorrect operands or expression!");
    }
    postfix.push(stack.top());
    stack.pop();
  }
  return postfix;
}

Expressions aleksandrov::getPostfixForms(Expressions& src)
{
  Expressions postfixes;
  while (!src.empty())
  {
    Expression expr(src.front());
    src.pop();
    Expression postfix(getPostfixForm(expr));
    postfixes.push(postfix);
  }
  return postfixes;
}

aleksandrov::OperandType aleksandrov::performOperation(OperationType op, OperandType a, OperandType b)
{
  using Op = OperationType;
  if (op == Op::Addition)
  {
    return sum(a, b);
  }
  else if (op == Op::Subtraction)
  {
    return sum(a, -b);
  }
  else if (op == Op::Multiplication)
  {
    return multiply(a, b);
  }
  else if (op == Op::Division)
  {
    return divide(a, b);
  }
  else if (op == Op::Modulo)
  {
    return modulo(a, b);
  }
  else
  {
    throw std::logic_error("Unsupported operation!");
  }
}

aleksandrov::OperandType aleksandrov::evalPostfixExpression(Expression& expr)
{
  Stack< OperandType > stack;
  if (expr.size() == 1)
  {
    ExpressionPart token(expr.front());
    expr.pop();
    if (token.isOperand())
    {
      return token.getOperand();
    }
  }
  while (!expr.empty())
  {
    ExpressionPart token(expr.front());
    expr.pop();
    if (token.isOperand())
    {
      stack.push(token.getOperand());
    }
    else if (token.isOperation())
    {
      if (stack.size() < 2)
      {
        throw std::logic_error("Incorrect operands!");
      }
      OperandType second = stack.top();
      stack.pop();
      OperandType first = stack.top();
      stack.pop();

      OperationType op = token.getOperation();
      OperandType result = performOperation(op, first, second);
      stack.push(result);
    }
  };
  if (stack.size() != 1)
  {
    throw std::logic_error("Incorrect expression!");
  }
  return stack.top();
}

Stack< OperandType > aleksandrov::evalPostfixExpressions(Expressions& src)
{
  Stack< OperandType > results;
  while (!src.empty())
  {
    Expression postfix(src.front());
    src.pop();
    OperandType result = evalPostfixExpression(postfix);
    results.push(result);
  }
  return results;
}

