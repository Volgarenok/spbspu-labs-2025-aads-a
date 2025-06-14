#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <vector>
#include <set>
#include "hashTable.hpp"

namespace maslov::detail
{
  struct PairHash1
  {
    size_t operator()(const std::pair< std::string, std::string > & pair) const
    {
      size_t h1 = std::hash< std::string >{}(pair.first);
      size_t h2 = std::hash< std::string >{}(pair.second);
      return h1 ^ h2;
    }
  };

  struct PairHash2
  {
    size_t operator()(const std::pair< std::string, std::string > & pair) const
    {
      boost::hash2::xxhash_64 hasher;
      hasher.update(pair.first.data(), pair.first.size());
      hasher.update(pair.second.data(), pair.second.size());
      return hasher.result();
    }
  };
}

namespace maslov
{
  struct Graph
  {
    void addEdge(const std::string & v1, const std::string & v2, int weight);
    void addVertex(const std::string & v);
    void deleteEdge(const std::string & v1, const std::string & v2, int weight);
    std::vector< std::string > getVertexes() const;
    bool hasVertex(const std::string & vertex) const;
    bool hasEdge(const std::string & v1, const std::string & v2, int weight) const;
    std::vector< std::pair< std::string, std::vector< int > > > getOutbound(const std::string & vertex) const;
    std::vector< std::pair< std::string, std::vector< int > > > getInbound(const std::string & vertex) const;
    void merge(const Graph & graph);
    void extract(const Graph & graph, const std::set< std::string > & extractVertexes);
   private:
    std::set< std::string > vertexes;
    HashTable< std::pair< std::string, std::string >, std::vector< int >, detail::PairHash1, detail::PairHash2 > edges;
  };
}

#endif
