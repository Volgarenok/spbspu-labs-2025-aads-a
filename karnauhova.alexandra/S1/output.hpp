#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <iostream>
#include <forward_list>
#include "fwd_list.hpp"

namespace karnauhova
{
  void output_names(karnauhova::Fwd_list<std::pair<std::string, karnauhova::Fwd_list<unsigned long long>>> l, std::ostream& out);
  void output_element_lists(karnauhova::Fwd_list<unsigned long long> lists, size_t index, std::ostream& out, bool& first);
  void output_lists(karnauhova::Fwd_list<std::pair<std::string, karnauhova::Fwd_list<unsigned long long>>> l, std::ostream& out);
  size_t max_lenght(karnauhova::Fwd_list<std::pair<std::string, karnauhova::Fwd_list<unsigned long long>>> l);
}

#endif
