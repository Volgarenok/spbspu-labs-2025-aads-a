#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include "AVLtree.hpp"

namespace tkach
{
  void print(std::istream& in, const AvlTree< std::string, AvlTree< size_t, std::string > >& avltree, std::ostream& out);
  void intersect(std::istream& in, AvlTree< std::string, AvlTree< size_t, std::string > >& avltree);
  void complement(std::istream& in, AvlTree< std::string, AvlTree< size_t, std::string > >& avltree);
  void unionTree(std::istream& in, AvlTree< std::string, AvlTree< size_t, std::string > >& avltree);
}

#endif
