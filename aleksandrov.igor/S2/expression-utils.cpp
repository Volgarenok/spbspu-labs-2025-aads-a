#include "expression-utils.hpp"
#include <iostream>
#include "expression-part.hpp"
#include "safe-math.hpp"
#include "stack.hpp"

namespace
{
  using namespace aleksandrov;

  bool isComplexOperation(OperationType op)
  {
    using Op = OperationType;
    return op == Op::Multiplication || op == Op::Division || op == Op::Modulus;
  }

  bool hasLowerPrecedence(OperationType a, OperationType b)
  {
    return !isComplexOperation(a) ? isComplexOperation(b) : false;
  }
}

std::istream& aleksandrov::operator>>(std::istream& in, ExpressionPart& token)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  if (!(in >> std::ws))
  {
    return in;
  }
  if (std::isdigit(in.peek()))
  {
    OperandType num;
    if (in >> num)
    {
      token = ExpressionPart(num);
    }
  }
  else
  {
    char c = '\0';
    in >> c;
    try
    {
      token = ExpressionPart(c);
    }
    catch (...)
    {
      in.setstate(std::ios::failbit);
    }
  }
  return in;
}

void aleksandrov::getExpressions(Expressions& exprs, std::istream& in)
{
  while ((in >> std::ws) && in.peek() != EOF)
  {
    Expression expr;
    while (in.peek() != '\n' && in.peek() != EOF)
    {
      ExpressionPart token('+');
      if (!(in >> token))
      {
        throw std::logic_error("Incorrect expression part!");
      }
      expr.push(token);
    }
    if (!expr.empty())
    {
      exprs.push(expr);
    }
    if (in.peek() == '\n')
    {
      in.get();
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
      while (!stack.empty() && stack.top().isOperation()
          && !hasLowerPrecedence(stack.top().getOperation(), token.getOperation()))
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

auto aleksandrov::performOperation(OperationType op, OperandType a, OperandType b) -> OperandType
{
  switch (op)
  {
  case OperationType::Addition:
    return safeSum(a, b);
  case OperationType::Subtraction:
    return safeSub(a, b);
  case OperationType::Multiplication:
    return safeMul(a, b);
  case OperationType::Division:
    return safeDiv(a, b);
  case OperationType::Modulus:
    return safeMod(a, b);
  }
  assert(false && "Unsupported operation type!");
  return 0;
}

aleksandrov::OperandType aleksandrov::evalPostfixExpression(Expression& expr)
{
  Stack< OperandType > stack;
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
      OperationType operation = token.getOperation();
      OperandType result = performOperation(operation, first, second);
      stack.push(result);
    }
  };
  if (stack.size() != 1)
  {
    throw std::logic_error("Incorrect expression!");
  }
  return stack.top();
}

