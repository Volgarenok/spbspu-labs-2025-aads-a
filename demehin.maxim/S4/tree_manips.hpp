#ifndef TREE_MANIPS
#define TREE_MANIPS
#include <string>
#include <iostream>
#include <tree/tree.hpp>

namespace demehin
{
  using TreeMap = demehin::Tree< size_t, std::string >;
  using MapOfTrees = demehin::Tree< std::string, TreeMap >;

  void print(std::ostream& out, std::istream& in, const MapOfTrees& mapOfTrees);
  void makeComplement(std::istream& in, MapOfTrees& mapOfTrees);
  void makeIntersect(std::istream& in, MapOfTrees& mapOfTrees);
  void makeUnion(std::istream& in, MapOfTrees& mapOfTrees);
}

#endif
