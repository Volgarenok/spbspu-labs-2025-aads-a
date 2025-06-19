#ifndef COUNT_SUM_HPP
#define COUNT_SUM_HPP

#include <iostream>
#include <forward_list>
#include "fwd_list.hpp"

namespace karnauhova
{
  int element_lists(karnauhova::Fwd_list<unsigned long long> lists, size_t index);
  void count_lists(karnauhova::Fwd_list<std::pair<std::string, karnauhova::Fwd_list<unsigned long long>>> l, std::ostream& out);
}

#endif
