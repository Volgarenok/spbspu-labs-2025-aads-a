#ifndef EXPR_URILS_HPP
#define EXPR_URILS_HPP

#include <ios>
#include <limits>
#include "queue.hpp"
#include "stack.hpp"
#include "infix-part.hpp"

namespace aleksandrov
{
  namespace detail
  {
    const long long int LLIMax = std::numeric_limits< long long int >::max();
    const long long int LLIMin = std::numeric_limits< long long int >::min();
    bool isLessImportant(const InfixPart&, const InfixPart&);
    int sign(long long int);
    bool sameSign(long long int, long long int);
  }
  long long int sum(long long int, long long int);
  long long int mult(long long int, long long int);
  long long int div(long long int, long long int);
  long long int mod(long long int, long long int);
  Queue< InfixPart > infixToPostfix(Queue< InfixPart >&);
  Queue< Queue< InfixPart > > infixesToPostfixes(Queue< Queue< InfixPart > >&);
  long long int evalPostfix(Queue< InfixPart >&);
  Stack< long long int > evalPostfixExprs(Queue< Queue< InfixPart > >&);
  Queue< InfixPart > getInfixExpr(std::istream&);
  Queue< Queue< InfixPart > > getInfixExprs(std::istream&);
}

#endif

