#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include <string>
#include "BinarySearchTree.hpp"

namespace nikonov
{
  using BasicTree = BinarySearchTree< int, std::string >;
  using TreeOfTrees = BinarySearchTree< std::string, BasicTree >;
  void inputTrees(TreeOfTrees & trees, std::istream & input);
  void print(TreeOfTrees & trees, std::istream & in, std::ostream & out);
  void complement(TreeOfTrees & trees, std::istream & in, std::ostream & out);
  void intersect(TreeOfTrees & trees, std::istream & in, std::ostream & out);
  void unite(TreeOfTrees & trees, std::istream & in, std::ostream & out);
}
#endif
