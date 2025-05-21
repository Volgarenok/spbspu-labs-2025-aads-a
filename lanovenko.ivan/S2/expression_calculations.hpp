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
  long long calculateBinary(long long a, long long b, char operation);
  void pushCalculated(Stack < long long >& result, char operation);
  long long calcualtePostfix(Queue< std::string >& postfix);
}

#endif
