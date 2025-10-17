#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>
#include "hashtable.hpp"

namespace kushekbaev
{
  struct HashForPair
  {
    size_t operator()(const std::pair< std::string, std::string >& pair) const
    {
      return std::hash< std::string >{}(pair.first) ^ std::hash< std::string >{}(pair.second);
    }
  };

  struct Graph
  {
    using hashMapForEdges = HashTable< std::pair< std::string, std::string >, std::map< size_t, size_t >, HashForPair >;
    void add_vertex(const std::string& str);
    void add_edge(std::string first, std::string second, size_t weigth);
    void add_edges(const Graph& other);
    bool remove_edge(const std::string& first, const std::string& second, size_t weigth);
    std::set< std::string > get_vertexes() const;
    hashMapForEdges get_edges() const;
    bool has_vert(const std::string& str) const;
    std::map< std::string, std::map< size_t, size_t > > get_out_bound(const std::string& str) const;
    std::map< std::string, std::map< size_t, size_t > > get_in_bound(const std::string& str) const;

    private:
      hashMapForEdges edges_;
      std::set< std::string > vertexes_;
  };
}

#endif
