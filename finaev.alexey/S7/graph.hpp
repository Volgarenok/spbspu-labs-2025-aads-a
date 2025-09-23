#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <string>
#include <vector>
#include <unordered_map>
#include <AVLtree.hpp>
#include <hashTable.hpp>

namespace finaev
{
  struct HashForPair
  {
    size_t operator()(const std::pair< std::string, std::string >& pair) const
    {
      return std::hash< std::string >{}(pair.first) ^ std::hash< std::string >{}(pair.second);
    }
  };

  class Graph
  {
  public:
    using hashMapForEdges = HashTable< std::pair< std::string, std::string >, AVLtree< size_t, size_t >, HashForPair >;

    void addVertex(const std::string& str);
    void addEdge(std::string first, std::string second, size_t weigth);
    void addEdges(const Graph& other);
    bool removeEdge(const std::string& first, const std::string& second, size_t weigth);
    AVLtree< std::string, bool > getVertexes() const;
    hashMapForEdges getEdges() const;
    bool hasVert(const std::string& str) const;
    AVLtree< std::string, AVLtree< size_t, size_t > > getOutBound(const std::string& str) const;
    AVLtree< std::string, AVLtree< size_t, size_t > > getInBound(const std::string& str) const;
  private:
    hashMapForEdges edges_;
    AVLtree< std::string, bool > vertexes_;
  };
}

#endif
