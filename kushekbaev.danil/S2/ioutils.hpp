#ifndef IOUTILS_HPP
#define IOUTILS_HPP

#include <iosfwd>
#include <string>
#include "queue.hpp"
#include "stack.hpp"

namespace kushekbaev
{
  void splitExpr(const std::string& string, kushekbaev::Queue< std::string >& arithmeticQueue);
  void inputExpr(std::istream& input, kushekbaev::Queue< kushekbaev::Queue< std::string > >& queue);
  void output(std::ostream& output, kushekbaev::Stack< long long int >& results);
}

#endif
