#ifndef CONVERSION_TO_POSTFIX_HPP
#define CONVERSION_TO_POSTFIX_HPP

#include <string>
#include <stdexcept>
#include "queue.hpp"
#include "stack.hpp"

namespace lanovenko
{
  std::string parseDigit(const std::string& str, size_t& num);
  size_t getPriority(char c);
  void pushSign(Queue< std::string >& result, Stack< char >& operations, char sign);
  Queue< std::string > toPostfix(const std::string& infix);
}

#endif
