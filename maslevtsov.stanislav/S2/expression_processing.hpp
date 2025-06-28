#ifndef EXPRESSION_PROCESSING_HPP
#define EXPRESSION_PROCESSING_HPP

#include "postfix_token.hpp"
#include "stack.hpp"

namespace maslevtsov {
  using expression_queue = maslevtsov::Queue< maslevtsov::PostfixToken >;
  using results_stack = maslevtsov::Stack< long long >;

  void calculate_expressions(expression_queue& exp_queue, results_stack& results);
}

#endif
