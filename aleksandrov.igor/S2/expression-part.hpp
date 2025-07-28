#ifndef EXPRESSION_PART_HPP
#define EXPRESSION_PART_HPP

namespace aleksandrov
{
  enum class BracketType;
  enum class OperationType;
  using OperandType = long long int;

  enum class TokenType;

  class ExpressionPart;
}

enum class aleksandrov::BracketType
{
  Opening,
  Closing
};

enum class aleksandrov::OperationType
{
  Addition,
  Subtraction,
  Multiplication,
  Division,
  Modulo
};

enum class aleksandrov::TokenType
{
  Bracket,
  Operation,
  Operand
};

class aleksandrov::ExpressionPart
{
public:
  explicit ExpressionPart(char);
  explicit ExpressionPart(OperandType);

  TokenType getType() const noexcept;
  BracketType getBracket() const noexcept;
  OperationType getOperation() const noexcept;
  OperandType getOperand() const noexcept;

  bool isOpeningBracket() const noexcept;
  bool isClosingBracket() const noexcept;
  bool isOperation() const noexcept;
  bool isOperand() const noexcept;

private:
  TokenType type_;
  union
  {
    BracketType bracket_;
    OperationType operation_;
    OperandType operand_;
  };
};

#endif

