#ifndef EXPRESSION_UTILS_HPP
#define EXPRESSION_UTILS_HPP

#include <ios>
#include "stack.hpp"
#include "queue.hpp"
#include "expression-part.hpp"

namespace aleksandrov
{
  using Expression = Queue< ExpressionPart >;
  using Expressions = Queue< Expression >;

  std::istream& operator>>(std::istream&, ExpressionPart&);

  void getExpression(std::istream&, Expression&);
  void getExpressions(std::istream&, Expressions&);

  Expression getPostfixForm(Expression&);
  Expressions getPostfixForms(Expressions&);

  OperandType performOperation(OperationType, OperandType, OperandType);
  OperandType evalPostfixExpression(Expression&);
  Stack< OperandType > evalPostfixExpressions(Expressions&);
}

#endif

