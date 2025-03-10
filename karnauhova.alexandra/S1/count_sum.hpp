#ifndef COUNT_SUM_HPP
#define COUNT_SUM_HPP

#include <iostream>
#include <forward_list>
#include "list.hpp"

namespace karnauhova
{
  int element_lists(karnauhova::List<unsigned long long> lists, size_t index);
  void count_lists(karnauhova::List<std::pair<std::string, karnauhova::List<unsigned long long>>> l, std::ostream& out);
}

#endif
