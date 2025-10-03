#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>

#include <hashtable/hash_table.hpp>

#include "list/CircularFwdList.hpp"
#include "tree/tree.hpp"

namespace zholobov {

  struct StringPairHash {
    size_t operator()(const std::pair< std::string, std::string >& p) const
    {
      size_t seed = 0;
      hash_combine(seed, p.first);
      hash_combine(seed, p.second);
      return seed;
    }

  private:
    static void hash_combine(size_t& seed, const std::string& value)
    {
      std::hash< std::string > hasher;
      seed ^= hasher(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
  };

  using EdgeHash = StringPairHash;

  class Graph {
  public:
    using Vertex = std::string;
    using Vertices = CircularFwdList< Vertex >;
    using Weight = unsigned int;
    using Edge = std::pair< Vertex, Vertex >;
    using WeightsList = CircularFwdList< Weight >;

    void addVertex(const Vertex& v);
    void addEdge(const Vertex& from, const Vertex& to, Weight weight);
    bool hasVertex(const Vertex& v) const;
    bool hasEdge(const Vertex& from, const Vertex& to, Weight weight) const;
    void removeEdge(const Vertex& from, const Vertex& to, Weight weight);
    Tree< Vertex, WeightsList > getOutbound(const Vertex& from) const;
    Tree< Vertex, WeightsList > getInbound(const Vertex& to) const;
    Vertices getVertices() const;
    size_t vertexCount() const;
    const HashTable< Edge, WeightsList, EdgeHash >& getAllEdges() const;

  private:
    HashTable< Edge, WeightsList, EdgeHash > edges;
    Tree< Vertex, bool > vertices;
  };

}

#endif
