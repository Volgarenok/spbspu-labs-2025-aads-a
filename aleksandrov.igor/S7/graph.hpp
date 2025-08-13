#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <unordered_map>
#include <map>
#include <vector>

namespace aleksandrov
{
  struct Edge
  {
    std::string dest;
    size_t weight;
  };

  using AdjacentyList = std::vector< Edge >;

  struct Vertex
  {
    std::string name;
    AdjacentyList adjacentyList;
  };

  using Vertices = std::map< std::string, Vertex >;

  struct Graph
  {
    std::string name;
    Vertices vertices;
  };
}

#endif

