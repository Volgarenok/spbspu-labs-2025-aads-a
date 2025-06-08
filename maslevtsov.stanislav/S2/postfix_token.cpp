#include "postfix_token.hpp"
#include <sstream>
#include "stack.hpp"
#include "checked_operations.hpp"

namespace {
  bool is_operation(const std::string& str) noexcept
  {
    return str == "*" || str == "/" || str == "%" || str == "+" || str == "-";
  }

  int get_precedence(char op)
  {
    switch (op) {
    case '*':
    case '/':
    case '%':
      return 1;
    case '+':
    case '-':
      return 2;
    default:
      return 3;
    }
  }

  bool precedence_comp(const std::string& left, const std::string& right) noexcept
  {
    return get_precedence(left[0]) <= get_precedence(right[0]);
  }
}

maslevtsov::PostfixToken::PostfixToken(const std::string& infix_token):
  token_()
{
  Stack< std::string > dump;
  std::size_t start = 0;
  while (start < infix_token.length()) {
    std::size_t end = infix_token.find(' ', start);
    std::string element = "";
    if (end == std::string::npos) {
      element = infix_token.substr(start);
      start = infix_token.length();
    } else {
      element = infix_token.substr(start, end - start);
      start = end + 1;
    }
    if (element == "(") {
      dump.push(element);
    } else if (is_operation(element)) {
      while (!dump.empty() && (precedence_comp(dump.top(), element))) {
        token_.push(dump.top());
        dump.pop();
      }
      dump.push(element);
    } else if (element == ")") {
      while (!dump.empty() && dump.top() != "(") {
        token_.push(dump.top());
        dump.pop();
      }
      if (dump.empty()) {
        throw std::logic_error("invalid expression");
      }
      dump.pop();
    } else {
      token_.push(element);
    }
  }
  while (!dump.empty()) {
    if (!is_operation(dump.top())) {
      throw std::logic_error("invalid expression");
    }
    token_.push(dump.top());
    dump.pop();
  }
  (*this)();
}

maslevtsov::PostfixToken maslevtsov::PostfixToken::operator+(const PostfixToken& other) const
{
  PostfixToken result(*this);
  return result.math_operator_impl(other, "+");
}

maslevtsov::PostfixToken maslevtsov::PostfixToken::operator-(const PostfixToken& other) const
{
  PostfixToken result(*this);
  return result.math_operator_impl(other, "-");
}

maslevtsov::PostfixToken maslevtsov::PostfixToken::operator*(const PostfixToken& other) const
{
  PostfixToken result(*this);
  return result.math_operator_impl(other, "*");
}

maslevtsov::PostfixToken maslevtsov::PostfixToken::operator/(const PostfixToken& other) const
{
  PostfixToken result(*this);
  return result.math_operator_impl(other, "/");
}

maslevtsov::PostfixToken maslevtsov::PostfixToken::operator%(const PostfixToken& other) const
{
  PostfixToken result(*this);
  return result.math_operator_impl(other, "%");
}

long long maslevtsov::PostfixToken::operator()() const
{
  Stack< long long > dump;
  Queue< std::string > exp = token_;
  while (!exp.empty()) {
    if (is_operation(exp.front())) {
      if (dump.size() < 2) {
        throw std::logic_error("invalid expression");
      }
      long long operand2 = dump.top();
      dump.pop();
      long long operand1 = dump.top();
      dump.pop();
      dump.push(checked_operation(operand1, operand2, exp.front()));
      exp.pop();
    } else {
      std::size_t pos = 0;
      long long operand = std::stoll(exp.front(), &pos);
      if (pos != exp.front().length()) {
        throw std::invalid_argument("invalid operand");
      }
      dump.push(operand);
      exp.pop();
    }
  }
  return dump.top();
}

maslevtsov::PostfixToken& maslevtsov::PostfixToken::math_operator_impl(const PostfixToken& other,
  const std::string& operation)
{
  PostfixToken copy(other);
  while (!copy.token_.empty()) {
    token_.push(copy.token_.front());
    copy.token_.pop();
  }
  token_.push(operation);
  return *this;
}
