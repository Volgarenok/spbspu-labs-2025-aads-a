#ifndef INPUT_TREES_HPP
#define INPUT_TREES_HPP
#include <istream>
#include <string>
#include <tree/avlTee.hpp>

namespace smirnov
{
  using BasicTree = AvlTree< int, std::string >;
  using TreeOfTrees = AvlTree< std::string, BasicTree >;
  void inputTrees(TreeOfTrees & trees, std::istream & input);
}
#endif
