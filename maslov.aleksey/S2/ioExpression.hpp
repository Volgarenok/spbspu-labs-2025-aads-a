#ifndef IO_EXPRESSION_HPP
#define IO_EXPRESSION_HPP

#include <iosfwd>
#include "queue.hpp"
#include "stack.hpp"

namespace maslov
{
  void inputFile(const std::string & filename, Queue< Queue< std::string > > & queue);
  void printData(std::ostream & out, Stack< long long int > & results);
}

#endif
