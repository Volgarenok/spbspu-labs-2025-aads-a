#ifndef INPUT_OUTPUT_DICTIONARIES_HPP
#define INPUT_OUTPUT_DICTIONARIES_HPP

#include <map>
#include <string>
#include <iostream>
#include <fstream>

namespace lanovenko
{
  using map_t = std::map< std::string, std::map< int, std::string > >;
  void getDictionaries(std::istream& in, map_t& dictionaries);
  void printDictionary(map_t& dictionaries);
}

#endif
