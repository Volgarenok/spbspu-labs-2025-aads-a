#include "expression_processing.hpp"

void maslevtsov::calculate_expressions(expression_stack& exp_stack, results_stack& results)
{
  expression_stack copy = exp_stack;
  while (!exp_stack.empty()) {
    results.push((exp_stack.top())());
    exp_stack.pop();
  }
  exp_stack = copy;
}
