#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <utility>
#include <tree.hpp>
#include <list.hpp>
#include <HashTable.hpp>

namespace duhanina
{
  struct EdgeHash
  {
    size_t operator()(const std::pair< std::string, std::string >& e) const
    {
      return std::hash< std::string >()(e.first) ^ (std::hash< std::string >()(e.second) << 1);
    }
  };

  class Graph
  {
  public:
    using Vertex = std::string;
    using Weight = unsigned int;
    using Edge = std::pair< Vertex, Vertex >;
    using WeightsList = List< Weight >;

    void addVertex(const Vertex& v);
    void addEdge(const Vertex& from, const Vertex& to, Weight weight);
    bool hasVertex(const Vertex& v) const;
    bool hasEdge(const Vertex& from, const Vertex& to, Weight weight) const;
    void removeEdge(const Vertex& from, const Vertex& to, Weight weight);
    Tree< Vertex, WeightsList, std::less< Vertex > > getOutbound(const Vertex& from) const;
    Tree< Vertex, WeightsList, std::less< Vertex > > getInbound(const Vertex& to) const;
    List< Vertex > getVertices() const;
    size_t vertexCount() const;
    const HashTable< Edge, WeightsList, EdgeHash >& getAllEdges() const;

  private:
    HashTable< Edge, WeightsList, EdgeHash > edges;
    Tree< Vertex, bool, std::less< Vertex > > vertices;
  };
}

#endif
