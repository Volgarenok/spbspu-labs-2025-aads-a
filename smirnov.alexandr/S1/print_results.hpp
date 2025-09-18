#ifndef PRINT_RESULTS_HPP
#define PRINT_RESULTS_HPP
#include <iostream>
#include <string>
#include <list/list.hpp>

namespace smirnov
{
  void printName(std::ostream & out, const List< std::pair< std::string, List< size_t > > > & sequences);
  void printSequences(std::ostream & out, const List< List< size_t > > & sequences);
  void printSums(std::ostream & out, const List< List< size_t > > & sequences);
}
#endif
