#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <map>
#include <ostream>

namespace mozhegova
{
  using dictionaries = std::map< std::string, std::map< int, std::string > >;
  void print(std::ostream & out, std::string name, const dictionaries & dicts);
  void complement(std::string newDictName, std::string dictName1, std::string dictName2, dictionaries & dicts);
  void intersect(std::string newDictName, std::string dictName1, std::string dictName2, dictionaries & dicts);
  void unionCmd(std::string newDictName, std::string dictName1, std::string dictName2, dictionaries & dicts);
}

#endif
