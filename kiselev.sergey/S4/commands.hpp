#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include <string>
#include "tree.hpp"

namespace kiselev
{
  using data = RBTree< size_t, std::string >;
  using dataset = RBTree< std::string, data >;
  void print(std::ostream&, std::istream&, const dataset&);
  void complement(std::istream&, dataset&);
  void intersect(std::istream&, dataset&);
  void unite(std::istream&, dataset&);
}
#endif
