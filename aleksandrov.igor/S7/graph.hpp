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
    AdjacentyList adjacentyList;
  };

  using Vertices = std::map< std::string, Vertex >;

  struct Graph
  {
  public:
    std::string name;
    Vertices vertices;

    void bind(const std::string& from, const std::string& to, size_t weight);
    void cut(const std::string& from, const std::string& to, size_t weight);

  private:
    using AdjListIterator = AdjacentyList::iterator;

    AdjListIterator findEdge(const std::string& from, const std::string& to, size_t weight);
  };
}

#endif

