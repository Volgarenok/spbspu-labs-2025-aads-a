#ifndef IO_UTILS_HPP
#define IO_UTILS_HPP

#include <string>
#include "queue.hpp"
#include "stack.hpp"

namespace kushekbaev
{
  void openFile(const std::string& filename, Queue< Queue< std::string > >& Q);
  void output(std::ostream& out, Stack< long long int >& results);
}

#endif
