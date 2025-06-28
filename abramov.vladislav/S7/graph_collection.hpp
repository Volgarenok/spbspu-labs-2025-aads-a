#ifndef GRAPH_COLLECTION_HPP
#define GRAPH_COLLECTION_HPP
#include <ostream>
#include <algorithm>
#include "graph.hpp"

namespace abramov
{
  struct GraphCollection
  {
    GraphCollection();
    GraphCollection(const GraphCollection &other);
    GraphCollection &operator=(const GraphCollection &other);
    void addGraph(const Graph &graph);
    Graph &getGraph(const std::string &name);
    const Graph &cgetGraph(const std::string &name) const;
    SimpleArray< std::string > getAllGraphsSorted() const;
    void printAllGraphs(std::ostream &out) const;
  private:
    HashTable< std::string, Graph > graphs;

    void swap(GraphCollection &other) noexcept;
  };
}
#endif
