#ifndef CONVERSION_HPP
#define CONVERSION_HPP
#include <string>
#include "queue.hpp"
#include "stack.hpp"

namespace smirnov
{
  smirnov::Queue< std::string > infixToPostfix(const std::string & infix);
  long long evaluatePostfix(Queue< std::string > & postfix);
}
#endif
