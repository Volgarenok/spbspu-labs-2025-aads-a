#include "postfix_token.hpp"
#include "stack.hpp"
#include "checked_operations.hpp"

namespace {
  bool is_num(const std::string& str) noexcept
  {
    for (auto i = str.cbegin(); i != str.cend(); ++i) {
      if (!std::isdigit(*i)) {
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
    return 3;
  }
}

maslevtsov::PostfixToken::PostfixToken(const std::string& infix_token):
  token_()
{
  Stack< std::string > dump;
  std::string element = "";
  for (auto i = infix_token.cbegin(); i != infix_token.cend(); ++i) {
    if (!std::isspace(*i)) {
      element += *i;
      if (i != --infix_token.cend()) {
        continue;
      }
    }
    if (element == "") {
      continue;
    }
    if (element == "(") {
      dump.push(element);
    } else if (is_num(element)) {
      token_.push(element);
    } else if (is_operation(element)) {
      while (!dump.empty() && (get_precedence(dump.top()) <= get_precedence(element))) {
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
      throw std::logic_error("invalid expression");
    }
    element.clear();
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
      dump.push(checked_operation(operand1, operand2, exp.front()));
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
