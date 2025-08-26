#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <hashTable.hpp>
#include <tree.hpp>

namespace mozhegova
{
  struct HashPair
  {
    size_t operator()(const std::pair< std::string, std::string > & pair) const
    {
      return std::hash< std::string >()(pair.first) ^ std::hash< std::string >()(pair.second);
    }
  };

  class Graph
  {
  public:
    void addEdge(const std::string & v1, const std::string & v2, size_t weight);
    void addVertex(const std::string & v);
    bool hasVert(const std::string & v) const;
    BiTree< std::string, bool > getVertexes() const;
    HashTable< std::pair< std::string, std::string >, BiTree< size_t, size_t >, HashPair > getEdges() const;
    BiTree< std::string, BiTree< size_t, size_t > > getOutBounds(const std::string & v) const;
    BiTree< std::string, BiTree< size_t, size_t > > getInBounds(const std::string & v) const;
    bool removeEdge(const std::string & v1, const std::string & v2, size_t weigh);
    void addEdges(const Graph & graph);
  private:
    HashTable< std::pair< std::string, std::string >, BiTree< size_t, size_t >, HashPair > edges;
    BiTree< std::string, bool > vertexes;
  };
}

#endif
