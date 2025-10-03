#ifndef CHECKED_OPERATIONS_HPP
#define CHECKED_OPERATIONS_HPP

#include <string>

namespace maslevtsov {
  long long checked_addition(long long left, long long right);
  long long checked_subtraction(long long left, long long right);
  long long checked_multiplication(long long left, long long right);
  long long checked_division(long long left, long long right);
  long long checked_remainder(long long left, long long right);
  long long checked_operation(long long left, long long right, const std::string& operation);
}

#endif
