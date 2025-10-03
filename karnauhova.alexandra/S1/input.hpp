#ifndef INPUT_HPP
#define INPUT_HPP

#include <iostream>
#include <string>
#include <forward_list>
#include "fwd_list.hpp"

namespace karnauhova
{
  std::pair< std::string, karnauhova::FwdList< unsigned long long > > input_line(std::istream& in, std::string name);
}

#endif
