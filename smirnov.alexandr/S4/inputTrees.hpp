#ifndef INPUT_TREES_HPP
#define INPUT_TREES_HPP

#include <istream>
#include <map>
#include <string>

namespace smirnov
{
  using BasicTree = std::map< int, std::string >;
  using TreeOfTrees = std::map< std::string, BasicTree >;
  void inputTrees(TreeOfTrees & trees, std::istream & input);
}
#endif
