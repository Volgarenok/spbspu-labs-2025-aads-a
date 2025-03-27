#include "parse_expr.hpp"
#include <cctype>
#include <stdexcept>
#include <climits>
#include "Stack.hpp"

namespace {
  bool is_operator(const std::string& str) {
    return str == "+" || str == "-" || str == "*" || str == "/" || str == "%";
  }

  int precedence(const std::string& op) {
    if (op == "+" || op == "-") {
      return 1;
    }
    if (op == "*" || op == "/" || op == "%") {
      return 2;
    }
    return 0;
  }

  bool is_number(const std::string& str)
  {
    if (str.empty()) {
      return false;
    }
    try {
      std::stoull(str);
      return true;
    } catch (const std::invalid_argument& e) {
      return false;
    } catch (const std::out_of_range& e) {
      return false;
    }
  }

  bool is_operand(const std::string& str) {
    return is_number(str);
  }

  long long do_operation(long long first, long long second, std::string oper)
  {
    if (oper == "+") {

      if (first > LLONG_MAX - second) {
        throw std::overflow_error("Overflow occurred during addition.");
      }
      return first + second;

    } else if (oper == "-") {

      if (first < LLONG_MIN + second) {
        throw std::overflow_error("Overflow occurred during subtraction.");
      }
      return first - second;

    } else if (oper == "*") {

      if (first > 0 && second > 0 && first > LLONG_MAX / second) {
        throw std::overflow_error("Overflow occurred during multiplication.");
      } else if (first < 0 && second < 0 && first < LLONG_MAX / second) {
          throw std::overflow_error("Overflow occurred during multiplication.");
      } else if (first > 0 && second < 0 && second < LLONG_MIN / first) {
          throw std::overflow_error("Overflow occurred during multiplication.");
      } else if (first < 0 && second > 0 && first < LLONG_MIN / second) {
          throw std::overflow_error("Overflow occurred during multiplication.");
      }
      return first * second;

    } else if (oper == "/") {

      if (second == 0) {
        throw std::invalid_argument("Division by zero is not allowed.");
      }
      if (first == LLONG_MIN && second == -1) {
          throw std::overflow_error("Overflow occurred during division by -1.");
      }
      return first / second;

    } else if (oper == "%") {

      if (second == 0) {
        throw std::invalid_argument("Modulo by zero is not allowed.");
      }
      if (first < 0) {
        return second - (first * (-1)) % second;
      }
      return first % second;

    } else {
      throw std::invalid_argument("Unknown operation");
    }
  }
}

gavrilova::Queue< std::string > gavrilova::split(const std::string& line, const char& symb)
{
  std::string next;
  Queue< std::string > queue;

  for (auto it = line.begin(); it != line.end(); ++it) {
    if (*it == symb) {
      if (!next.empty()) {
        queue.push(next);
        next.clear();
      }
    } else {
      next += *it;
    }
  }
  if (!next.empty()) {
    queue.push(next);
  }
  return queue;
}

gavrilova::Queue< std::string > gavrilova::infix_to_postfix(Queue< std::string >& infix_q)
{
  Stack< std::string > stack;
  Queue< std::string > postfix_q;

  while (!infix_q.empty()) {
    std::string token = infix_q.front();
    infix_q.pop();

    if (is_operand(token)) {
      postfix_q.push(token);
    } else if (token == "(") {
      stack.push(token);
    } else if (token == ")") {
      while (!stack.empty() && stack.top() != "(") {
        postfix_q.push(stack.top());
        stack.pop();
      }
      if (!stack.empty() && stack.top() == "(") {
        stack.pop();
      } else {
        throw std::invalid_argument("Invalid placement of brackets in expression");
      }
    } else if (is_operator(token)) {
      while (!stack.empty() && precedence(stack.top()) >= precedence(token)) {
        postfix_q.push(stack.top());
        stack.pop();
      }
      stack.push(token);
    } else {
      throw std::invalid_argument("Invalid value in expression");
    }
  }

  while (!stack.empty()) {
    postfix_q.push(stack.top());
    stack.pop();
  }

  return postfix_q;
}

long long gavrilova::calculate(Queue< std::string >& postfix_q)
{
  Stack< long long > stack;
  while (!postfix_q.empty()) {
    std::string token = postfix_q.front();
    postfix_q.pop();

    if (is_operand(token)) {
      stack.push(std::stoull(token));
    } else if (is_operator(token)) {
      if (stack.size() < 2) {
        throw std::invalid_argument("Invalid expression: not enough operands");
      }
      long long second = stack.top();
      stack.pop();
      long long first = stack.top();
      stack.pop();
      stack.push(do_operation(first, second, token));
    } else {
      throw std::invalid_argument("Invalid value in expression");
    }
  }

  if (stack.size() != 1) {
    throw std::invalid_argument("Invalid expression: incorrect number of operands");
  }

  long long result = stack.top();
  stack.pop();
  return result;
}
