#ifndef ACTIONSEXPR_HPP
#define ACTIONSEXPR_HPP

#include "queue.hpp"
#include "stack.hpp"
#include <iostream>
#include <ostream>
#include <string>

namespace bocharov
{
  using queue = Queue< std::string >;
  using stack = Stack< std::string >;
  using stackNumber = Stack< long long int >;
  queue convertExpr(queue & infix);
  stackNumber calculationExpr(queue & postfix);
  void inputExprs(std::istream & input, queue & exprs);
  std::ostream & outputResults(std::ostream & output, stackNumber & results);
}

#endif
