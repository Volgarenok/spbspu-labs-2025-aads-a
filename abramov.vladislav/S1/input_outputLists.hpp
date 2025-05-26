#ifndef INPUT_OUTPUTLISTS_HPP
#define INPUT_OUTPUTLISTS_HPP
#include <fstream>
#include "list.hpp"

namespace abramov
{
  using list_of_lists = List< std::pair < std::string, List< unsigned long long int > > >*;
  std::istream &inputLists(std::istream &in, list_of_lists lists, size_t &lists_count, size_t &count);
  std::ostream &printNames(std::ostream &out, const list_of_lists lists);
  std::ostream &printValues(std::ostream &out, const list_of_lists lists, size_t count);
}
#endif
