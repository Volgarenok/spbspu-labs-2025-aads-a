#include "infix-part.hpp"

#include <cassert>

namespace aleksandrov
{
  InfixPart::InfixPart(char c)
  {
    assert( c == '(' || c == ')' || c == '+' || c == '-' || c == '*' || c == '/' || c == '%');
    if (c == '(')
    {
      type_ = Type::OpeningBracket;
      token_.openingBracket = c;
    }
    else if (c == ')')
    {
      type_ = Type::ClosingBracket;
      token_.closingBracket = c;
    }
    else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%')
    {
      type_ = Type::Operation;
      token_.operation = c;
    }
  }

  InfixPart::InfixPart(long long int num):
    type_(Type::Operand)
  {
    token_.operand = num;
  }

  bool InfixPart::isOpeningBracket() const noexcept
  {
    return type_ == Type::OpeningBracket;
  }

  bool InfixPart::isClosingBracket() const noexcept
  {
    return type_ == Type::ClosingBracket;
  }

  bool InfixPart::isOperation() const noexcept
  {
    return type_ == Type::Operation;
  }

  bool InfixPart::isOperand() const noexcept
  {
    return type_ == Type::Operand;
  }

  InfixPart::Type InfixPart::getType() const noexcept
  {
    return type_;
  }

  char InfixPart::getOpeningBracket() const noexcept
  {
    assert(isOpeningBracket());
    return token_.openingBracket;
  }

  char InfixPart::getClosingBracket() const noexcept
  {
    assert(isClosingBracket());
    return token_.closingBracket;
  }

  char InfixPart::getOperation() const noexcept
  {
    assert(isOperation());
    return token_.operation;
  }

  long long int InfixPart::getOperand() const noexcept
  {
    assert(isOperand());
    return token_.operand;
  }
}

