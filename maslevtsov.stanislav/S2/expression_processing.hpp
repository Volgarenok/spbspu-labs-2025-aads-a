#ifndef EXPRESSION_PROCESSING_HPP
#define EXPRESSION_PROCESSING_HPP

#include "postfix_token.hpp"
#include "stack.hpp"

namespace maslevtsov {
  using expression_stack = maslevtsov::Stack< maslevtsov::PostfixToken >;
  using results_stack = maslevtsov::Stack< long long >;

  void calculate_expressions(expression_stack& exp_stack, results_stack& results);
}

#endif
