#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <string>
#include <cstddef>
#include <hash_table/hashTable.hpp>

namespace smirnov
{
  class Graph
  {
  public:
    Graph() = default;
    Graph(const std::string & name);
    const std::string & getName() const noexcept;
    bool hasVertex(const std::string & vertex) const;
    bool addVertex(const std::string & vertex);
    bool addEdge(const std::string & from, const std::string & to, size_t weight);
    bool removeEdge(const std::string & from, const std::string & to, size_t weight);
    std::vector< std::string > getVertices() const;
    std::vector< std::pair< std::string, size_t > > getOutboundEdges(const std::string & vertex) const;
    std::vector< std::pair< std::string, size_t > > getInboundEdges(const std::string & vertex) const;
  private:
    std::string name_;
    HashTable< std::string, HashTable< std::string, std::vector< size_t > > > edges_;
  };
}
#endif
