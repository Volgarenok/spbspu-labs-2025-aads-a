#include "io_stack.hpp"

void maslevtsov::input_expressions(std::istream& in, expression_stack& exp_stack)
{
  std::string str;
  while (!in.eof()) {
    std::getline(in, str);
    if (str.empty()) {
      continue;
    }
    expression_queue exp_queue;
    std::string element;
    for (auto i = str.cbegin(); i != str.cend(); ++i) {
      if (std::isspace(*i)) {
        exp_queue.push(element);
        element.clear();
        continue;
      } else {
        element += *i;
      }
    }
    if (element != "") {
      exp_queue.push(element);
    }
    if (!exp_queue.empty()) {
      exp_stack.push(exp_queue);
    }
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
