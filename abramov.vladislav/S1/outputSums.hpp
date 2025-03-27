#ifndef OUTPUTSUMS_HPP
#define OUTPUTSUMS_HPP
#include <fstream>
#include "list.hpp"

namespace abramov
{
  using list_of_lists = List< std::pair < std::string, List< unsigned long long int > > >*;
  std::ostream &outputSums(std::ostream &out, const list_of_lists lists, size_t count);
}
#endif

