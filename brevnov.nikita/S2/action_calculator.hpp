#ifndef ACTION_CALCULATOR_HPP
#define ACTION_CALCULATOR_HPP
#include "queue.hpp"
#include "stack.hpp"
#include <iostream>
#include <string>

namespace brevnov
{
  using expression = Queue< std::string >;
  using queue = Queue< expression >;
  using stack = Stack< std::string >;
  using stack_number = Stack< long long int >;

  queue convert_expression(queue& infix);
  stack_number calculation_expression(queue& postfix);
  void input_expression(std::istream& input, queue& exprs);
  std::ostream& output_results(std::ostream& output, stack_number& results);
}
#endif
