#ifndef EXPRESSION_PROCESSING_HPP
#define EXPRESSION_PROCESSING_HPP

#include <string>
#include "queue.hpp"
#include "stack.hpp"

namespace maslevtsov {
  using expression_queue = maslevtsov::Queue< std::string >;
  using expression_stack = maslevtsov::Stack< expression_queue >;
  using results_stack = maslevtsov::Stack< long long >;

  void convert(expression_queue& exp);
  void convert_stack(expression_stack& exp_stack);
  long long calculate_expression(const expression_queue& exp_queue);
  void calculate_expressions(expression_stack& exp_stack, results_stack& results);
}

#endif
