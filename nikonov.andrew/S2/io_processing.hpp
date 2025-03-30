#ifndef IO_POCESSING_HPP
#define IO_POCESSING_HPP
#include <string>
#include <istream>
#include <ostream>
#include "stack.hpp"
namespace nikonov
{
  Queue< std::string > readInfixExpressions(std::istream & in);
  Stack< std::string > convertToPostfix(Queue< std::string > queue);
  std::string convertToPostfix(const std::string & infix);
  void printResults(Stack< std::string > toOut, std::ostream & out);
  bool isOperand(const std::string & el);
  bool isOperator(const std::string & el);
  bool isLoverPrecedence(const std::string & a, const std::string & b);
}
#endif
