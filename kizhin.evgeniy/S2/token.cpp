#include "token.hpp"
#include <cassert>
#include <iostream>

kizhin::Token::Token():
  type_(TokenType::unknown)
{}

kizhin::Token::Token(const number_type number):
  type_(TokenType::number),
  number_(number)
{}

kizhin::Token::Token(const BinaryOperation* operation):
  type_(TokenType::operation),
  operation_(operation)
{}

kizhin::Token::Token(const BracketType bracket):
  type_(TokenType::bracket),
  bracket_(bracket)
{}

kizhin::TokenType kizhin::Token::type() const noexcept
{
  return type_;
}

kizhin::Token::number_type kizhin::Token::number() const noexcept
{
  constexpr TokenType number = TokenType::number;
  assert(type() == number && "Access to number in non-number token");
  return number_;
}

const kizhin::BinaryOperation* kizhin::Token::operation() const noexcept
{
  constexpr TokenType operation = TokenType::operation;
  assert(type() == operation && "Access to operation in non-operation token");
  return operation_;
}

kizhin::BracketType kizhin::Token::bracket() const noexcept
{
  constexpr TokenType bracket = TokenType::bracket;
  assert(type() == bracket && "Access to bracket in non-bracket token");
  return bracket_;
}

