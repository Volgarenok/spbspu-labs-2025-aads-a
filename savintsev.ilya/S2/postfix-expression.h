#ifndef POSTFIX_EXPRESSION_H
#define POSTFIX_EXPRESSION_H
#include <string>
#include "queue.hpp"

namespace savintsev
{
  class PostfixExpr
  {
  public:
    long long operator()() const;

    PostfixExpr operator+(const PostfixExpr & rhs);
    PostfixExpr operator-(const PostfixExpr & rhs);
    PostfixExpr operator*(const PostfixExpr & rhs);
    PostfixExpr operator/(const PostfixExpr & rhs);
    PostfixExpr operator%(const PostfixExpr & rhs);

    friend PostfixExpr convert(const std::string & infix);
  private:
    Queue< std::string > expr_;

    PostfixExpr & use_operator(const std::string & op, PostfixExpr rhs);
  };

  PostfixExpr convert(const std::string & infix);
}

#endif
