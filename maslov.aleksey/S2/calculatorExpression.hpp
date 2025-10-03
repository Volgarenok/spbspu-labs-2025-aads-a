#ifndef CALCULATOR_EXPRESSION_HPP
#define CALCULATOR_EXPRESSION_HPP

#include <string>
#include <queue.hpp>
#include <stack.hpp>

namespace maslov
{
  long long int calculatePostfix(Queue< std::string > postfixQueue);
  Queue< std::string > infixToPostfix(Queue< std::string > infixQueue);
}

#endif
