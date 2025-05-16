#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include "tree.hpp"

namespace maslov
{
  using Dictionaries = BiTree< std::string,
      BiTree< int, std::string, std::less< int > >, std::less< std::string > >;
  void inputFile(const std::string & filename, Dictionaries & dicts);
  void processCommand(std::istream & in, const std::string & command, Dictionaries & dicts);
}

#endif
