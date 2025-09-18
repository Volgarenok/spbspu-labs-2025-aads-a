#include "expression-part.hpp"

#include <cassert>
#include <stdexcept>

namespace
{
  using aleksandrov::BracketType;
  using aleksandrov::OperationType;
  using aleksandrov::TokenType;

  TokenType getExpressionPartType(char c)
  {
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%')
    {
      return TokenType::Operation;
    }
    else if (c == '(' || c == ')')
    {
      return TokenType::Bracket;
    }
    throw std::logic_error("Incorrect expression part!");
  }

  OperationType getOperationType(char c) noexcept
  {
    switch (c)
    {
    case '+':
      return OperationType::Addition;
    case '-':
      return OperationType::Subtraction;
    case '*':
      return OperationType::Multiplication;
    case '/':
      return OperationType::Division;
    default:
      return OperationType::Modulus;
    }
  }

  BracketType getBracketType(char c) noexcept
  {
    switch (c)
    {
    case '(':
      return BracketType::Opening;
    default:
      return BracketType::Closing;
    }
  }
}

aleksandrov::ExpressionPart::ExpressionPart(char c):
  type_(::getExpressionPartType(c))
{
  if (type_ == TokenType::Operation)
  {
    operation_ = getOperationType(c);
  }
  else
  {
    bracket_ = getBracketType(c);
  }
}

aleksandrov::ExpressionPart::ExpressionPart(OperandType operand):
  type_(TokenType::Operand),
  operand_(operand)
{}

aleksandrov::TokenType aleksandrov::ExpressionPart::getType() const noexcept
{
  return type_;
}

aleksandrov::BracketType aleksandrov::ExpressionPart::getBracket() const noexcept
{
  assert(type_ == TokenType::Bracket);
  return bracket_;
}

aleksandrov::OperationType aleksandrov::ExpressionPart::getOperation() const noexcept
{
  assert(type_ == TokenType::Operation);
  return operation_;
}

aleksandrov::OperandType aleksandrov::ExpressionPart::getOperand() const noexcept
{
  assert(type_ == TokenType::Operand);
  return operand_;
}

bool aleksandrov::ExpressionPart::isOpeningBracket() const noexcept
{
  return type_ == TokenType::Bracket && bracket_ == BracketType::Opening;
}

bool aleksandrov::ExpressionPart::isClosingBracket() const noexcept
{
  return type_ == TokenType::Bracket && bracket_ == BracketType::Closing;
}

bool aleksandrov::ExpressionPart::isOperation() const noexcept
{
  return type_ == TokenType::Operation;
}

bool aleksandrov::ExpressionPart::isOperand() const noexcept
{
  return type_ == TokenType::Operand;
}

