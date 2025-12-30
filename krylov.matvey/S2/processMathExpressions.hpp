#ifndef PROCESSMATHEXPRESSIONS_HPP
#define PROCESSMATHEXPRESSIONS_HPP
#include <ios>
#include "queue.hpp"
#include "stack.hpp"

namespace krylov
{
  long long int calculatePostfix(Queue< std::string >* expr);
  std::istream& calculateExpression(std::istream& in);
  long long int calculateInfix(const std::string& str);
  Queue< std::string >* getPostfixExpression(const std::string& str);
}

#endif
