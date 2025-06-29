#ifndef GRAPH_UTILS_HPP
#define GRAPH_UTILS_HPP
#include <fstream>
#include "graph_collection.hpp"

namespace abramov
{
  std::ifstream &readGraphs(std::ifstream &in, GraphCollection &collect);
  void printVertsSorted(const GraphCollection &collect, const std::string &name, std::ostream &out);
  void printOutVerts(const GraphCollection &collect, const std::string &n, const std::string &v, std::ostream &out);
  void printInVerts(const GraphCollection &collect, const std::string &n, const std::string &v, std::ostream &out);
  void addEdgeToGraph(GraphCollection &collect, const std::string &name, std::istream &in);
  void cutEdgeFromGraph(GraphCollection &collect, const std::string &name, std::istream &in);
  void createGraph(GraphCollection &collect, std::istream &in);
  void mergeGraphs(GraphCollection &collect, const std::string &name, std::istream &in);
}
#endif
