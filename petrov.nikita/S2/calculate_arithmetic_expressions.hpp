#ifndef CALCULATE_ARITHMETIC_EXPRESSIONS_HPP
#define CALCULATE_ARITHMETIC_EXPRESSIONS_HPP

#include <string>
#include <queue>
#include "stack.hpp"

namespace petrov
{
  petrov::Stack< long long int > calculateArithmeticExpressions(std::queue< std::string > & queue);
  std::queue< std::string > transformInfixToPostfix(std::queue< std::string > & queue);
  long long int calculatePostfixExpression(std::queue< std::string > & queue);
}

#endif
