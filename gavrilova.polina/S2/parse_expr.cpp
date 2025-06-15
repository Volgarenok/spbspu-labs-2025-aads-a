#include "parse_expr.hpp"
#include <cctype>
#include <climits>
#include <stdexcept>
#include <Stack.hpp>

namespace {
  bool is_operator(const std::string& str)
  {
    return str == "+" || str == "-" || str == "*" || str == "/" || str == "%";
  }

  bool has_higher_precedence(const std::string& op1, const std::string& op2)
  {
    auto get_precedence = [](const std::string& op)
    {
      if (op == "+" || op == "-") {
        return 1;
      }
      if (op == "*" || op == "/" || op == "%") {
        return 2;
      }
      return 0;
    };
    return get_precedence(op1) >= get_precedence(op2);
  }

  long long plus(long long a, long long b)
  {
    if (a > LLONG_MAX - b) {
      throw std::overflow_error("Overflow in addition");
    }
    return a + b;
  }

  long long subtract(long long a, long long b)
  {
    if ((b > 0 && a < LLONG_MIN + b) || (b < 0 && a > LLONG_MAX + b)) {
      throw std::overflow_error("Overflow in subtraction");
    }
    return a - b;
  }

  long long multiply(long long a, long long b)
  {
    if (a > 0 && b > 0 && a > LLONG_MAX / b) {
      throw std::overflow_error("Overflow in multiplication");
    }
    if (a < 0 && b < 0 && a < LLONG_MAX / b) {
      throw std::overflow_error("Overflow in multiplication");
    }
    if (a > 0 && b < 0 && b < LLONG_MIN / a) {
      throw std::overflow_error("Overflow in multiplication");
    }
    if (a < 0 && b > 0 && a < LLONG_MIN / b) {
      throw std::overflow_error("Overflow in multiplication");
    }
    return a * b;
  }

  long long divide(long long a, long long b)
  {
    if (b == 0) {
      throw std::invalid_argument("Division by zero");
    }
    if (a == LLONG_MIN && b == -1) {
      throw std::overflow_error("Overflow in division");
    }
    return a / b;
  }

  long long mod(long long a, long long b)
  {
    if (b == 0) {
      throw std::invalid_argument("Modulo by zero");
    }
    if (a < 0) {
      return b - (-a) % b;
    }
    return a % b;
  }

  long long do_operation(long long first, long long second, std::string oper)
  {
    if (oper == "+") {
      return plus(first, second);
    } else if (oper == "-") {
      return subtract(first, second);
    } else if (oper == "*") {
      return multiply(first, second);
    } else if (oper == "/") {
      return divide(first, second);
    } else if (oper == "%") {
      return mod(first, second);
    } else {
      throw std::invalid_argument("Unknown operation");
    }
  }
}

gavrilova::Queue< std::string > gavrilova::split(const std::string& line, const char& symb)
{
  Queue< std::string > queue;
  size_t start = 0;
  size_t end = line.find(symb);

  while (end != std::string::npos) {
    if (end != start) {
      queue.push(line.substr(start, end - start));
    }
    start = end + 1;
    end = line.find(symb, start);
  }

  if (start < line.length()) {
    queue.push(line.substr(start));
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

    if (token == "(") {
      stack.push(token);
    } else if (token == ")") {
      while (!stack.empty() && stack.top() != "(") {
        postfix_q.push(stack.top());
        stack.pop();
      }
      if (stack.empty()) {
        throw std::invalid_argument("Mismatched parentheses");
      }
      stack.pop();
    } else if (is_operator(token)) {
      while (!stack.empty() && stack.top() != "(" && has_higher_precedence(stack.top(), token)) {
        postfix_q.push(stack.top());
        stack.pop();
      }
      stack.push(token);
    } else {
      postfix_q.push(token);
    }
  }

  while (!stack.empty()) {
    if (stack.top() == "(") {
      throw std::invalid_argument("Mismatched parentheses");
    }
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

    if (is_operator(token)) {
      if (stack.size() < 2) {
        throw std::invalid_argument("Invalid expression: not enough operands");
      }
      long long second = stack.top();
      stack.pop();
      long long first = stack.top();
      stack.pop();
      stack.push(do_operation(first, second, token));
    } else {
      try {
        stack.push(std::stoll(token));
      } catch (...) {
        throw std::invalid_argument("Invalid operand: " + token);
      }
    }
  }

  if (stack.size() != 1) {
    throw std::invalid_argument("Invalid expression: incorrect number of operands");
  }

  long long result = stack.top();
  stack.pop();
  return result;
}
