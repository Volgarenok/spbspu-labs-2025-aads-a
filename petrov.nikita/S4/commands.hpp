#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <iostream>
#include "avl_tree.hpp"

namespace petrov
{
  using subtree_t = AVLTree< int, std::string, std::less< int > >;
  using maintree_t = AVLTree< std::string, subtree_t >;

  std::ostream & print(std::ostream & out, std::istream & in, const maintree_t & tree);
  void complement(std::istream & in, maintree_t & tree);
  void intersect(std::istream & in, maintree_t & tree);
  void unionCMD(std::istream & in, maintree_t & tree);
}

#endif
