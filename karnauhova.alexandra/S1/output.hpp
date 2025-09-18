#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <iostream>
#include <forward_list>
#include "fwd_list.hpp"

namespace karnauhova
{
  void output_names(karnauhova::FwdList< std::pair< std::string, karnauhova::FwdList< unsigned long long > > > l, std::ostream& out);
  void output_element_lists(karnauhova::FwdList< unsigned long long > lists, size_t index, std::ostream& out, bool& first);
  void output_lists(karnauhova::FwdList< std::pair< std::string, karnauhova::FwdList< unsigned long long > > > l, std::ostream& out);
  size_t max_lenght(karnauhova::FwdList< std::pair< std::string, karnauhova::FwdList< unsigned long long > > > l);
}

#endif
