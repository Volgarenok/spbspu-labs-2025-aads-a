#ifndef EXPRESSION_UTILS_HPP
#define EXPRESSION_UTILS_HPP

#include <ios>
#include "queue.hpp"
#include "expression-part.hpp"

namespace aleksandrov
{
  using Expression = Queue< ExpressionPart >;
  using Expressions = Queue< Expression >;

  std::istream& operator>>(std::istream&, ExpressionPart&);

  void getExpressions(Expressions&, std::istream&);
  Expression getPostfixForm(Expression&);
  OperandType performOperation(OperationType, OperandType, OperandType);
  OperandType evalPostfixExpression(Expression&);
}

#endif

