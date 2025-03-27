#ifndef SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S2_POSTFIX_EXPRESSION_HPP
#define SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S2_POSTFIX_EXPRESSION_HPP

#include <queue.hpp>
#include "token.hpp"

namespace kizhin {
  class PostfixExpression;

  PostfixExpression operator+(const PostfixExpression&, const PostfixExpression&);
  PostfixExpression operator-(const PostfixExpression&, const PostfixExpression&);
  PostfixExpression operator*(const PostfixExpression&, const PostfixExpression&);
  PostfixExpression operator/(const PostfixExpression&, const PostfixExpression&);
}

class kizhin::PostfixExpression final
{
public:
  using number_type = Token::number_type;

  PostfixExpression(Queue< Token >);

  PostfixExpression& operator+=(const PostfixExpression&);
  PostfixExpression& operator-=(const PostfixExpression&);
  PostfixExpression& operator*=(const PostfixExpression&);
  PostfixExpression& operator/=(const PostfixExpression&);

  size_t size() const noexcept;
  bool empty() const noexcept;

  number_type evaluate() const;

  void clear() noexcept;
  void swap(PostfixExpression&) noexcept;

private:
  Queue< Token > tokens_;

  void appendExpression(const PostfixExpression&, const BinaryOperation*);
};

#endif

