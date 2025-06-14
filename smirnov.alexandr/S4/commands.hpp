#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <string>
#include <map>

namespace smirnov
{
  using BasicTree = std::map< int, std::string >;
  using TreeOfTrees = std::map< std::string, BasicTree >;
  void print(const TreeOfTrees & trees, std::istream & in, std::ostream & out);
  void complement(TreeOfTrees & trees, std::istream & in, std::ostream & out);
  void intersect(TreeOfTrees & trees, std::istream & in, std::ostream & out);
  void unite(TreeOfTrees & trees, std::istream & in, std::ostream & out);
}
#endif
