#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <iostream>
#include <forward_list>
#include "list.hpp"

namespace karnauhova
{
  void output_names(karnauhova::List<std::pair<std::string, karnauhova::List<unsigned long long>>> l, std::ostream& out);
  void output_element_lists(karnauhova::List<unsigned long long> lists, size_t index, std::ostream& out, bool& first);
  void output_lists(karnauhova::List<std::pair<std::string, karnauhova::List<unsigned long long>>> l, std::ostream& out);
  size_t max_lenght(karnauhova::List<std::pair<std::string, karnauhova::List<unsigned long long>>> l);
}

#endif
