#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <tree.hpp>

namespace mozhegova
{
  using dictionaries = BiTree< std::string, BiTree< int, std::string > >;
  void print(std::istream & in, std::ostream & out, const dictionaries & dicts);
  void complement(std::istream & in, dictionaries & dicts);
  void intersect(std::istream & in, dictionaries & dicts);
  void unionCmd(std::istream & in, dictionaries & dicts);
}

#endif
