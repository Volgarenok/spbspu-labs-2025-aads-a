#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <set>
#include <string>
#include <vector>
#include <stdexcept>
#include "hash_table.hpp"


namespace lanovenko
{
  struct PairHash1
  {
    size_t operator()(const std::pair< std::string, std::string >& pair) const
    {
      size_t h1 = std::hash< std::string >{}(pair.first);
      size_t h2 = std::hash< std::string >{}(pair.second);
      return h1 ^ h2;
    }
  };

  struct PairHash2
  {
    size_t operator()(const std::pair< std::string, std::string >& pair) const
    {
      boost::hash2::xxhash_64 hasher;
      hasher.update(pair.first.data(), pair.first.size());
      hasher.update(pair.second.data(), pair.second.size());
      return hasher.result();
    }
  };

  class Graph
  {
  public:
    void add_edge(const std::string& v1, const std::string& v2, int weight);
    void add_vertex(const std::string& v);
    void delete_edge(const std::string& v1, const std::string& v2, int weight);
    std::vector< std::string > get_vertexes() const;
    bool has_vertex(const std::string& vertex) const;
    bool has_edge(const std::string& v1, const std::string& v2, int weight) const;
    std::vector< std::pair< std::string, std::vector< int > > > get_outbound(const std::string& vertex) const;
    std::vector< std::pair< std::string, std::vector< int > > > get_inbound(const std::string& vertex) const;
    void merge(const Graph& graph);
    void extract(const Graph& graph, const std::set< std::string >& extractVertexes);
  private:
    std::set< std::string > vertexes;
    HashTable< std::pair< std::string, std::string >, std::vector< int >, PairHash1, PairHash2 > edges;
  };
}

#endif
