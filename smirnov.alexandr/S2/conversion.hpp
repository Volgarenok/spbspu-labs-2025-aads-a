#ifndef CONVERSION_HPP
#define CONVERSION_HPP
#include <string>
#include <utils/queue.hpp>

namespace smirnov
{
  Queue< std::string > infixToPostfix(const std::string & infix);
  long long evaluatePostfix(Queue< std::string > & postfix);
}
#endif
