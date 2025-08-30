#include "checked_operations.hpp"
#include <limits>
#include <stdexcept>

namespace {
  constexpr long long max_ll = std::numeric_limits< long long >::max();
  constexpr long long min_ll = std::numeric_limits< long long >::min();

  int sign(long long value)
  {
    return (value > 0) ? 1 : ((value < 0) ? -1 : 0);
  }

  bool same_sign(long long left, long long right)
  {
    return sign(left) * sign(right) > 0;
  }
}

long long maslevtsov::checked_addition(long long left, long long right)
{
  if (same_sign(left, right) && (left > 0)) {
    if (max_ll - left >= right) {
      return left + right;
    }
  } else if (same_sign(left, right) && (left < 0)) {
    if (min_ll - left <= right) {
      return left + right;
    }
  } else if (!same_sign(left, right)) {
    return left + right;
  }
  throw std::overflow_error("addition overflow");
}

long long maslevtsov::checked_subtraction(long long left, long long right)
{
  if ((right > 0 && left < min_ll + right) || (right < 0 && left > max_ll + right)) {
    throw std::overflow_error("subtraction overflow");
  }
  return left - right;
}

long long maslevtsov::checked_multiplication(long long left, long long right)
{
  if (same_sign(left, right) && left > 0) {
    if (max_ll / left > right) {
      return left * right;
    }
  } else if (same_sign(left, right) && left < 0) {
    if (min_ll / left < right) {
      return left * right;
    }
  } else if (!same_sign(left, right)) {
    if (std::abs(max_ll / left) > std::abs(right)) {
      return left * right;
    }
  }
  throw std::overflow_error("multiplication overflow");
}

long long maslevtsov::checked_division(long long left, long long right)
{
  if (right == 0) {
    throw std::logic_error("division by zero");
  }
  if ((left == -1 && right == min_ll) || (left == min_ll && right == -1)) {
    throw std::overflow_error("division overflow");
  }
  return left / right;
}

long long maslevtsov::checked_remainder(long long left, long long right)
{
  if (right == 0) {
    throw std::logic_error("division by zero");
  }
  return (right + (left % right)) % right;
}

long long maslevtsov::checked_operation(long long left, long long right, const std::string& operation)
{
  if (operation.size() > 1) {
    throw std::logic_error("invalid operation");
  }
  char op = operation[0];
  switch (op) {
  case '*':
    return checked_multiplication(left, right);
    break;
  case '/':
    return checked_division(left, right);
  case '%':
    return checked_remainder(left, right);
    break;
  case '+':
    return checked_addition(left, right);
  case '-':
    return checked_subtraction(left, right);
    break;
  default:
    throw std::logic_error("invalid operation");
  }
}
