#ifndef IO_STACK_HPP
#define IO_STACK_HPP

#include <iostream>
#include "stack.hpp"
#include "postfix_token.hpp"

namespace maslevtsov {
  using expression_queue = maslevtsov::Queue< maslevtsov::PostfixToken >;
  using results_stack = maslevtsov::Stack< long long >;

  void input_expressions(std::istream& in, expression_queue& exp_queue);
  void output_results(std::ostream& out, results_stack& results) noexcept;
}

#endif
