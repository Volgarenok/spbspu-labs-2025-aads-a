#ifndef PRINT_RESULTS_HPP
#define PRINT_RESULTS_HPP
#include <iostream>
#include <string>
#include <list>

namespace smirnov
{
  void printName(std::ostream & out, const std::list< std::pair< std::string, std::list< size_t > > > & sequences);
  void printSequences(std::ostream & out, const std::list< std::list< size_t > > & sequences);
  void printSums(std::ostream & out, const std::list< std::list< size_t > > & sequences);
}
#endif
