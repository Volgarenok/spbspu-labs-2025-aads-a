#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <iostream>
#include <tree.hpp>

using dataset_t = kushekbaev::Tree< size_t, std::string >;
using dict_t = kushekbaev::Tree< std::string, dataset_t >;

namespace kushekbaev
{
  void print(std::ostream& out, std::istream& in, const dict_t& dictionary);
  void complement(std::istream& in, dict_t& dictionary);
  void intersect(std::istream& in, dict_t& dictionary);
  void unification(std::istream& in, dict_t& dictionary);
}

#endif
