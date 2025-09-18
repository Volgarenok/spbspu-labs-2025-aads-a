#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include <string>
#include <tree/avlTee.hpp>

namespace smirnov
{
  using BasicTree = AvlTree< int, std::string >;
  using TreeOfTrees = AvlTree< std::string, BasicTree >;
  void print(const TreeOfTrees & trees, std::istream & in, std::ostream & out);
  void complement(TreeOfTrees & trees, std::istream & in);
  void intersect(TreeOfTrees & trees, std::istream & in);
  void unite(TreeOfTrees & trees, std::istream & in);
}
#endif
