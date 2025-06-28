#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <boost/container_hash/hash.hpp>
#include <boost/container_hash/hash_fwd.hpp>
#include <cstddef>
#include <unordered_map>
#include <set>
#include <list>
#include <map>
#include <string>
#include <functional>
#include <utility>
#include <vector>
#include "hashTable.hpp"
namespace detail
{
  struct PairHash1
  {
    size_t operator()(const std::pair< std::string, std::string >& pair) const
    {
      size_t h = std::hash< std::string >()(pair.first);
      boost::hash_combine(h, pair.second);
      return h;
    }
  };

  struct PairHash2
  {
    size_t operator()(const std::pair< std::string, std::string >& pair) const
    {
      size_t h = kiselev::BoostHash< std::string >()(pair.first);
      boost::hash_combine(h, pair.second);
      return h;
    }
  };
}

namespace kiselev
{
  class Graph
  {
  public:
    using Vertexes = std::pair< std::string, std::string >;
    using Edge = HashTable< Vertexes, std::list< unsigned int >, detail::PairHash1, detail::PairHash2 >;
    using MapHash = std::map< std::string, HashTable< unsigned int, unsigned int, std::hash< unsigned int >, BoostHash< unsigned int > > >;

    void addEdge(const std::string& v1, const std::string& v2, unsigned int weight)
    {
      edges[{ v1, v2 }].push_back(weight);
    }

    bool removeEdge(const std::string& v1, const std::string& v2, unsigned int weight)
    {
      auto it = edges.find({ v1, v2 });
      if (it == edges.end())
      {
        return false;
      }
      size_t size1 = it->second.size();
      it->second.remove(weight);
      size_t size2 = it->second.size();
      if (it->second.empty())
      {
        edges.erase(it);
      }
      return size2 < size1;
    }

    std::set< std::string > getVertexes() const
    {
      std::set< std::string > vertexes;
      for (auto it = edges.cbegin(); it != edges.cend(); ++it)
      {
        vertexes.insert(it->first.first);
        vertexes.insert(it->first.second);
      }
      return vertexes;
    }

    MapHash getOutBound(const std::string& v) const
    {
      return getBound(v, true);
    }
    MapHash getInBound(const std::string& v) const
    {
      return getBound(v, false);
    }

    const Edge& getEdges() const
    {
      return edges;
    }
  private:
    Edge edges;
    MapHash getBound(const std::string& v, bool isOut) const
    {
      MapHash result;
      for (auto it = edges.cbegin(); it != edges.cend(); ++it)
      {
        const std::string& current = isOut ? it->first.first : it->first.second;
        if (current == v)
        {
          std::string other = isOut ? it->first.second : it->first.first;
          for (auto weightIt = it->second.begin(); weightIt != it->second.end(); ++weightIt)
          {
            result[other][*weightIt]++;
          }
        }
      }
      return result;
    }
  };
}
#endif
