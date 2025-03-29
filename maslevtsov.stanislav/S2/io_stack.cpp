#include "io_stack.hpp"

void maslevtsov::input_expressions(std::istream& in, expression_stack& exp_stack)
{
  std::string str;
  while (!in.eof()) {
    std::getline(in, str);
    if (str.empty()) {
      continue;
    }
    PostfixToken token(str);
    exp_stack.push(token);
  }
}

void maslevtsov::output_results(std::ostream& out, results_stack& results) noexcept
{
  out << results.top();
  results.pop();
  while (!results.empty()) {
    out << " " << results.top();
    results.pop();
  }
}
