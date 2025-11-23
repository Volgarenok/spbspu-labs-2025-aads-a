#ifndef GRAPH_UTILS_HPP
#define GRAPH_UTILS_HPP

#include <ios>
#include <string>
#include "graph.hpp"

namespace aleksandrov
{
  using Graphs = Tree< std::string, Graph >;

  void readGraphs(std::istream&, Graphs&);
  void processCommands(std::istream&, std::ostream&, Graphs&);
}

#endif

