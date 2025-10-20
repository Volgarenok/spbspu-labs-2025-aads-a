#ifndef EXPRESSION_CALCULATIONS_HPP
#define EXPRESSION_CALCULATIONS_HPP

#include <limits>
#include <stdexcept>
#include <string>
#include "stack.hpp"
#include "queue.hpp"

namespace lanovenko
{
  bool additionOverflow(long long a, long long b);
  bool multiplicationOverflow(long long a, long long b);
  long long calculateBinary(long long a, long long b, const std::string& operation);
  void pushCalculated(Stack < long long >& result, const std::string& operation);
  long long calcualtePostfix(Queue< std::string >& postfix);
}

#endif
