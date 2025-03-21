#include <fstream>
#include "io_stack.hpp"
#include "expression_processing.hpp"

namespace {
  using expression_queue = maslevtsov::Queue< std::string >;
  using expression_stack = maslevtsov::Stack< expression_queue >;
  using results_stack = maslevtsov::Stack< long long >;
}

int main(int argc, char** argv)
{
  if (argc > 2) {
    std::cerr << "Invalid amount of arguments\n";
    return 1;
  }
  expression_stack exp_stack;
  try {
    if (argc == 1) {
      maslevtsov::input_expressions(std::cin, exp_stack);
    }
    if (argc == 2) {
      std::ifstream fin(argv[1]);
      maslevtsov::input_expressions(fin, exp_stack);
    }
  } catch (const std::bad_alloc&) {
    std::cerr << "Memory allocation error\n";
    return 1;
  }

  if (exp_stack.empty()) {
    std::cout << '\n';
    return 0;
  }

  try {
    maslevtsov::convert_stack(exp_stack);
  } catch (const std::exception&) {
    std::cerr << "Invalid expression\n";
    return 1;
  }
  results_stack results;
  try {
    maslevtsov::calculate_expressions(exp_stack, results);
  } catch (const std::exception&) {
    std::cerr << "Calculation error\n";
    return 1;
  }

  maslevtsov::output_results(std::cout, results);
  std::cout << '\n';
}
