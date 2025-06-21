#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <tree/tree.hpp>

namespace maslov
{
  using Dictionaries = BiTree< std::string,
      BiTree< int, std::string, std::less< int > >, std::less< std::string > >;
  void inputFile(const std::string & filename, Dictionaries & dicts);
  void printCommand(std::istream & in, std::ostream & out, const Dictionaries & dicts);
  void complementCommand(std::istream & in, Dictionaries & dicts);
  void intersectCommand(std::istream & in, Dictionaries & dicts);
  void unionCommand(std::istream & in, Dictionaries & dicts);
}

#endif
