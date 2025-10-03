#ifndef CMDS_HPP
#define CMDS_HPP

#include <ostream>
#include <istream>
#include <string>
#include <map>
#include <avlTree.hpp>

namespace dribas
{
  using data = AVLTree< size_t, std::string >;
  using dataset = AVLTree< std::string, data >;

  void print(std::ostream&, std::istream&, const dataset&);
  void complement(std::istream&, dataset&);
  void intersect(std::istream&, dataset&);
  void unite(std::istream&, dataset&);
}

#endif
