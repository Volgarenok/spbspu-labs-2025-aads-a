#ifndef IOUTILS_HPP
#define IOUTILS_HPP

#include <iosfwd>
#include <string>
#include "queue.hpp"
#include "stack.hpp"

namespace kushekbaev
{
  void splitExpr(const std::string& string, Queue< std::string >& arithmeticQueue);
  void inputExpr(std::istream& input, Queue< Queue< std::string > >& queue);
  void output(std::ostream& output, const Stack< long long int >& results);
}

#endif
