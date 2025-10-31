#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <boost/container_hash/hash.hpp>
#include <set>
#include <list>
#include <map>
#include <string>
#include "hashTable.hpp"
namespace detail
{
  struct PairHash
  {
    size_t operator()(const std::pair< std::string, std::string >& pair) const
    {
      size_t h = std::hash< std::string >()(pair.first);
      boost::hash_combine(h, pair.second);
      return h;
    }
  };
}

namespace karnauhova
{
  class Graph
  {
  public:
    using Vertexes = std::pair< std::string, std::string >;
    using Edge = HashTable< Vertexes, std::list< unsigned int >, detail::PairHash >;
    using MapHash = std::map< std::string, HashTable< unsigned int, unsigned int, std::hash< unsigned int >>>;\

    void addEdge(const std::string& v1, const std::string& v2, unsigned int weight);
    bool removeEdge(const std::string& v1, const std::string& v2, unsigned int weight);

    std::set< std::string > getVertexes() const;
    MapHash getOutBound(const std::string& v) const;
    MapHash getInBound(const std::string& v) const;

    const Edge& getEdges() const;
  private:
    Edge edges;
    MapHash getBound(const std::string& v, bool isOut) const;
  };
}

#endif
