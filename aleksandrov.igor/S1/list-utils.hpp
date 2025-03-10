#ifndef LIST_UTILS_HPP
#define LIST_UTILS_HPP

#include "list.hpp"
#include <ios>

namespace aleksandrov
{
  unsigned long long calcSum(List< unsigned long long >&);
  void outputList(List< unsigned long long >&, std::ostream&);
}

#endif

