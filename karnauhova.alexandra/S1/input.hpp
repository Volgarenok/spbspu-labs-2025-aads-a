#ifndef INPUT_HPP
#define INPUT_HPP

#include <iostream>
#include <string>
#include <forward_list>
#include "list.hpp"

namespace karnauhova
{
  std::pair<std::string, karnauhova::List<unsigned long long>> input_line(std::istream& in, std::string name);
}

#endif
