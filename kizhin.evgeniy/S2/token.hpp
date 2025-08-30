#ifndef SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S2_TOKEN_HPP
#define SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S2_TOKEN_HPP

#include <iosfwd>
#include "operations.hpp"

namespace kizhin {
  enum class TokenType;
  enum class BracketType;

  class Token;
}

enum class kizhin::TokenType {
  number,
  operation,
  bracket,
  unknown,
};

enum class kizhin::BracketType {
  opening,
  closing,
};

class kizhin::Token final
{
public:
  using number_type = BinaryOperation::number_type;
  Token();
  explicit Token(number_type);
  explicit Token(const BinaryOperation*);
  explicit Token(BracketType);

  TokenType type() const noexcept;
  number_type number() const noexcept;
  const BinaryOperation* operation() const noexcept;
  BracketType bracket() const noexcept;

private:
  TokenType type_;
  union {
    number_type number_;
    const BinaryOperation* operation_;
    BracketType bracket_;
  };
};

#endif

