#ifndef COUNT_SUM_HPP
#define COUNT_SUM_HPP

#include <iostream>
#include <forward_list>
#include "fwd_list.hpp"

namespace karnauhova
{
  int element_lists(karnauhova::FwdList< unsigned long long > lists, size_t index);
  void count_lists(karnauhova::FwdList< std::pair< std::string, karnauhova::FwdList< unsigned long long > > > l, std::ostream& out);
}

#endif
