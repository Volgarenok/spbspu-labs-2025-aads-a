#ifndef INPUTTREES_HPP
#define INPUTTREES_HPP

#include <istream>
#include <string>
#include <UBST/UBST.hpp>

namespace shramko
{
  using BasicTree = UBstTree< int, std::string >;
  using TreeOfTrees = UBstTree< std::string, BasicTree >;
  void inputTrees(TreeOfTrees & trees, std::istream & input);
}

#endif
