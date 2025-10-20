#ifndef CMDS_HPP
#define CMDS_HPP
#include <two-three-tree.h>

using DataTree = savintsev::TwoThreeTree< long long, std::string >;

namespace savintsev
{
  template< typename T >
  void ascending(DataTree & tree, T smth)
  {
    smth = tree.traverse_lnr(smth);
  }
  template< typename T >
  void descending(DataTree & tree, T smth)
  {
    smth = tree.traverse_rnl(smth);
  }
  template< typename T >
  void breadth(DataTree & tree, T smth)
  {
    smth = tree.traverse_breadth(smth);
  }
}

#endif
