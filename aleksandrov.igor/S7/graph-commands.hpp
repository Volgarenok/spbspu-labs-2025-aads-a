#ifndef GRAPH_COMMANDS_HPP
#define GRAPH_COMMANDS_HPP

#include "graph.hpp"

namespace aleksandrov
{
  using Graphs = std::map< std::string, Graph >;

  void graphs(const Graphs&, std::ostream&);
  void vertexes(const Graphs&, std::istream&, std::ostream&);
  void outBound(const Graphs&, std::istream&, std::ostream&);
  void inBound(const Graphs&, std::istream&, std::ostream&);
  void bind(Graphs&, std::istream&);
  void cut(Graphs&, std::istream&);
  void create(Graphs&, std::istream&);
  void merge(Graphs&, std::istream&);
  void extract(Graphs&, std::istream&);
}

#endif

