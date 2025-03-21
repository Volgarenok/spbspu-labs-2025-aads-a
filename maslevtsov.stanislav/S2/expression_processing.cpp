#include "expression_processing.hpp"
#include <stdexcept>
#include "checked_operations.hpp"

namespace {
  bool is_num(const std::string& str) noexcept
  {
    for (auto i = str.cbegin(); i != str.cend(); ++i) {
      if (!isdigit(*i)) {
        return false;
      }
    }
    return true;
  }

  bool is_operation(const std::string& str) noexcept
  {
    return str == "*" || str == "/" || str == "%" || str == "+" || str == "-";
  }

  int get_precedence(const std::string& operation) noexcept
  {
    if (operation == "*" || operation == "/" || operation == "%") {
      return 1;
    }
    if (operation == "+" || operation == "-") {
      return 2;
    }
    return -1;
  }
}

void maslevtsov::convert(expression_queue& exp)
{
  maslevtsov::Stack< std::string > dump;
  expression_queue result;
  while (!exp.empty()) {
    if (exp.front() == "(") {
      dump.push(exp.front());
      exp.pop();
    } else if (is_num(exp.front())) {
      result.push(exp.front());
      exp.pop();
    } else if (is_operation(exp.front())) {
      while (!dump.empty() && (get_precedence(dump.top()) >= get_precedence(exp.front()))) {
        result.push(dump.top());
        dump.pop();
      }
      dump.push(exp.front());
      exp.pop();
    } else if (exp.front() == ")") {
      while (!dump.empty() && dump.top() != "(") {
        result.push(dump.top());
        dump.pop();
      }
      if (dump.empty()) {
        throw std::logic_error("invalid expression");
      }
      dump.pop();
      exp.pop();
    } else {
      throw std::logic_error("invalid expression");
    }
  }
  while (!dump.empty()) {
    if (!is_operation(dump.top())) {
      throw std::logic_error("invalid expression");
    }
    result.push(dump.top());
    dump.pop();
  }
  exp = result;
}

void maslevtsov::convert_stack(expression_stack& exp_stack)
{
  expression_stack converted;
  while (!exp_stack.empty()) {
    convert(exp_stack.top());
    converted.push(exp_stack.top());
    exp_stack.pop();
  }
  exp_stack = converted;
}

long long maslevtsov::calculate_expression(const expression_queue& exp_queue)
{
  maslevtsov::Stack< long long > dump;
  expression_queue exp = exp_queue;
  while (!exp.empty()) {
    if (is_num(exp.front())) {
      dump.push(std::stoll(exp.front()));
      exp.pop();
    } else {
      if (dump.size() < 2) {
        throw std::logic_error("invalid expression");
      }
      long long operand2 = dump.top();
      dump.pop();
      long long operand1 = dump.top();
      dump.pop();
      dump.push(maslevtsov::checked_operation(operand1, operand2, exp.front()));
      exp.pop();
    }
  }
  return dump.top();
}

void maslevtsov::calculate_expressions(expression_stack& exp_stack, results_stack& results)
{
  expression_stack copy = exp_stack;
  while (!exp_stack.empty()) {
    results.push(calculate_expression(exp_stack.top()));
    exp_stack.pop();
  }
  exp_stack = copy;
}
