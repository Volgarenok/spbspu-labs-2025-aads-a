#include "expression_processing.hpp"

void maslevtsov::calculate_expressions(expression_queue& exp_queue, results_stack& results)
{
  expression_queue copy = exp_queue;
  while (!exp_queue.empty()) {
    results.push((exp_queue.front())());
    exp_queue.pop();
  }
  exp_queue = copy;
}
