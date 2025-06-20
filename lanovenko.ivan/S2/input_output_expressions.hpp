#ifndef INPUT_OUTPUT_EXPRESSIONS_HPP
#define INPUT_OUTPUT_EXPRESSIONS_HPP

#include <istream>
#include <ostream>
#include <string>
#include "queue.hpp"
#include "stack.hpp"

namespace lanovenko
{
  void inputExpression(std::istream& in, Queue< std::string >& infixExpressions);
  void resultsOutput(Stack< long long >& results, std::ostream& out);
}

#endif
