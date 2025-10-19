#ifndef COMMANDPROCESSOR_HPP
#define COMMANDPROCESSOR_HPP

#include <iostream>
#include "commands.hpp"

namespace shramko
{
  void processCommand(TreeOfTrees & trees, const std::string & cmd, std::istream & in, std::ostream & out);
}

#endif
