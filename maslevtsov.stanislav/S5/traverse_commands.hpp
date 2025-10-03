#ifndef TRAVERSE_COMMANDS_HPP
#define TRAVERSE_COMMANDS_HPP

#include <tree/definition.hpp>

namespace maslevtsov {
  template< class F >
  void traverse_ascend(Tree< int, std::string >& tree, F pred)
  {
    pred = tree.traverse_lnr(pred);
  }

  template< class F >
  void traverse_descend(Tree< int, std::string >& tree, F pred)
  {
    pred = tree.traverse_rnl(pred);
  }

  template< class F >
  void traverse_breadth(Tree< int, std::string >& tree, F pred)
  {
    pred = tree.traverse_breadth(pred);
  }
}

#endif
