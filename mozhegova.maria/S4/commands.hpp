#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <ostream>
#include <tree.hpp>

namespace mozhegova
{
  using dictionaries = BiTree< std::string, BiTree< int, std::string > >;
  void print(std::ostream & out, std::string name, const dictionaries & dicts);
  void complement(std::string newDictName, std::string dictName1, std::string dictName2, dictionaries & dicts);
  void intersect(std::string newDictName, std::string dictName1, std::string dictName2, dictionaries & dicts);
  void unionCmd(std::string newDictName, std::string dictName1, std::string dictName2, dictionaries & dicts);
}

#endif
