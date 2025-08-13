#ifndef GRAPH_COMMANDS_HPP
#define GRAPH_COMMANDS_HPP

#include "graph.hpp"

namespace aleksandrov
{
  using Graphs = std::map< std::string, Graph >;

  void graphs(const Graphs&, std::ostream&);
  void vertexes(const Graphs&, std::istream&, std::ostream&);
  void outbound(const Graphs&, std::istream&, std::ostream&);
  void inbound(const Graphs&, std::istream&, std::ostream&);
  void bind(Graphs&, std::istream&);
  void cut(Graphs&, std::istream&);
}

#endif

