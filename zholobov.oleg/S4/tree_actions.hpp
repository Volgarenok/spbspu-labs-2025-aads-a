#ifndef TREE_ACTIONS_HPP
#define TREE_ACTIONS_HPP

#include <iosfwd>

#include <tree/tree.hpp>

namespace zholobov {

  using TreeMap = zholobov::Tree< size_t, std::string >;
  using NamedTrees = zholobov::Tree< std::string, TreeMap >;

  void doPrint(std::ostream& out, std::istream& in, const NamedTrees& namedTrees);
  void doComplement(std::istream& in, NamedTrees& namedTrees);
  void doIntersect(std::istream& in, NamedTrees& namedTrees);
  void doUnion(std::istream& in, NamedTrees& namedTrees);

}

#endif
