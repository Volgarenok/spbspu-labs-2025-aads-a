#ifndef ACTIONSEXPR_HPP
#define ACTIONSEXPR_HPP

#include <iostream>
#include <string>
#include <stack.hpp>
#include <queue.hpp>

namespace bocharov
{
  using Expr = Queue< std::string >;
  using queue = Queue< Expr >;
  using stack = Stack< std::string >;
  using stackNumber = Stack< long long int >;
  queue convertExpr(queue & infix);
  stackNumber calculationExpr(queue & postfix);
  void inputExprs(std::istream & input, queue & exprs);
  std::ostream & outputResults(std::ostream & output, stackNumber & results);
}

#endif
