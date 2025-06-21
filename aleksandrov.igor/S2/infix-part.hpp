#ifndef INFIX_PART_HPP
#define INFIX_PART_HPP

namespace aleksandrov
{
  class InfixPart
  {
  public:
    enum class Type
    {
      OpeningBracket,
      ClosingBracket,
      Operation,
      Operand
    };

    explicit InfixPart(char);
    explicit InfixPart(long long int);

    bool isOpeningBracket() const noexcept;
    bool isClosingBracket() const noexcept;
    bool isOperation() const noexcept;
    bool isOperand() const noexcept;

    Type getType() const noexcept;
    char getOpeningBracket() const noexcept;
    char getClosingBracket() const noexcept;
    char getOperation() const noexcept;
    long long int getOperand() const noexcept;
  private:
    Type type_;
    union Token
    {
      char openingBracket;
      char closingBracket;
      char operation;
      long long int operand;
    } token_;
  };
}

#endif

