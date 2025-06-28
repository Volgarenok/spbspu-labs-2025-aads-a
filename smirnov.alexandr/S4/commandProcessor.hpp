#ifndef COMMAND_PROCESSOR_HPP
#define COMMAND_PROCESSOR_HPP

#include <iostream>
#include "commands.hpp"

namespace smirnov
{
  void processCommand(TreeOfTrees & trees, const std::string & cmd, std::istream & in, std::ostream & out);
}
#endif
