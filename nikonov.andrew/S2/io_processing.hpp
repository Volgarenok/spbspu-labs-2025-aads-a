#ifndef IO_POCESSING_HPP
#define IO_POCESSING_HPP
#include <string>
#include <istream>
#include <ostream>
#include "postfix.hpp"
namespace nikonov
{
  Queue< std::string > readInfixExpressions(std::istream & in);
  void printResults(Stack< Postfix > toOut, std::ostream & out);
}
#endif
