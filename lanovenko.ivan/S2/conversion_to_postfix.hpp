#ifndef CONVERSION_TO_POSTFIX_HPP
#define CONVERSION_TO_POSTFIX_HPP

#include <string>
#include <stdexcept>
#include "queue.hpp"
#include "stack.hpp"

namespace lanovenko
{
  bool isOperator(const std::string& lhs);
  bool comparePriorities(const std::string& lhs, const std::string& rhs);
  void pushSign(Queue< std::string >& result, Stack< std::string >& operations, const std::string& sign);
  Queue< std::string > toPostfix(std::string infix);
}

#endif
