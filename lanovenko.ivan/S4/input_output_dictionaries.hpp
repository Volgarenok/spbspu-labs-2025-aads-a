#ifndef INPUT_OUTPUT_DICTIONARIES_HPP
#define INPUT_OUTPUT_DICTIONARIES_HPP

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include "tree.hpp"

namespace lanovenko
{
  using map_t = Tree< std::string, Tree< int, std::string, std::less< int > >, std::less< std::string > >;
  void getDictionaries(std::istream& in, map_t&);
  void printDictionary(const map_t& dictionaries);
}

#endif
