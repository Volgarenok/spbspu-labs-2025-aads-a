#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <string>
#include <tree/tree.hpp>

namespace sveshnikov
{
  using Tree = AvlTree< int, std::string >;

  template< typename F >
  void ascend(Tree &data, F &f)
  {
    f = data.traverse_lnr(f);
  }

  template< typename F >
  void descend(Tree &data, F &f)
  {
    f = data.traverse_rnl(f);
  }

  template< typename F >
  void breadthTrav(Tree &data, F &f)
  {
    f = data.traverse_breadth(f);
  }
}

#endif
