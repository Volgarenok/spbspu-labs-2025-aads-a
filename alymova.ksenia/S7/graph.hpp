#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <tree/tree-2-3.hpp>
#include <list/list.hpp>
#include "hash-functions.hpp"
#include "hash-table.hpp"

namespace alymova
{
  using BoundMap = TwoThreeTree< std::string, List< size_t >, std::less< std::string > >;

  struct Graph
  {
    HashTable< std::pair< std::string, std::string >, size_t, PairHasher< std::string > > edges;
    List< std::string > vertexes;

    void addEdge(const std::string& vertex1, const std::string& vertex2, size_t weight);
    void cutEdge(const std::string& vertex1, const std::string& vertex2, size_t weight);
    void addVertex(const std::string& vertex);
    bool hasVertex(const std::string& vertex) const;
    BoundMap getOutbound(const std::string& vertex) const;
    BoundMap getInbound(const std::string& vertex) const;
    void merge(const Graph& other);
  };

  std::istream& operator>>(std::istream& in, Graph& graph);
  std::ostream& operator<<(std::ostream& out, const Graph& graph);
  std::ostream& operator<<(std::ostream& out, const List< std::string >& list);
  std::ostream& operator<<(std::ostream& out, const BoundMap& bound);
}

#endif
