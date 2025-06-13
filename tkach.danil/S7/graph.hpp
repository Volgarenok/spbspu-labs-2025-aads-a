#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <AVLtree.hpp>
#include "hash_table.hpp"
#include "dynamic_array.hpp"

namespace tkach
{
  struct HashPair
  {
    size_t operator()(const std::pair< std::string, std::string >& p) const
    {
      size_t h1 = std::hash< std::string >()(p.first);
      size_t h2 = std::hash< std::string >()(p.second);
      return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
  };

  class Graph
  {
  public:
    void addEdge(const std::string& vert1, const std::string& vert2, size_t weight);
    AvlTree< std::string, bool > getAllVert() const;
    AvlTree< std::string, AvlTree< size_t, size_t > > getOutBound(const std::string& vert) const;
    AvlTree< std::string, AvlTree< size_t, size_t > > getInBound(const std::string& vert) const;
    bool removeEdge(const std::string& vert1, const std::string& vert2, size_t weight);
    void addEdges(const Graph& other);
    bool hasVert(const std::string& vert_name) const;
    HashTable< std::pair< std::string, std::string >, AvlTree< size_t, size_t >, HashPair > gerGraphEdges() const;
  private:
    HashTable< std::pair< std::string, std::string >, AvlTree< size_t, size_t >, HashPair > edges_;
  };
}
#endif
