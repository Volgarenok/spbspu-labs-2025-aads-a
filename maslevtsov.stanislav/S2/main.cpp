#include <fstream>
#include "io_stack.hpp"
#include "expression_processing.hpp"

namespace {
  using expression_queue = maslevtsov::Queue< maslevtsov::PostfixToken >;
  using results_stack = maslevtsov::Stack< long long >;
}

int main(int argc, char** argv)
{
  if (argc > 2) {
    std::cerr << "Invalid amount of arguments\n";
    return 1;
  }
  expression_queue exp_queue;
  try {
    if (argc == 1) {
      maslevtsov::input_expressions(std::cin, exp_queue);
    }
    if (argc == 2) {
      std::ifstream fin(argv[1]);
      if (!fin) {
        std::cerr << "Invalid input file\n";
        return 1;
      }
      maslevtsov::input_expressions(fin, exp_queue);
    }
  } catch (const std::exception&) {
    std::cerr << "Incorrect input\n";
    return 1;
  }

  if (exp_queue.empty()) {
    std::cout << '\n';
    return 0;
  }

  results_stack results;
  try {
    maslevtsov::calculate_expressions(exp_queue, results);
  } catch (const std::exception&) {
    std::cerr << "Calculation error\n";
    return 1;
  }

  maslevtsov::output_results(std::cout, results);
  std::cout << '\n';
}
