#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include "graph.hpp"

namespace sveshnikov
{
  using GraphsMap_t = std::unordered_map< std::string, sveshnikov::Graph >;

  void graphs(const GraphsMap_t &graph_map, std::ostream &out);
  void vertexes(const GraphsMap_t &graph_map, std::istream &in, std::ostream &out);
  void outbound(const GraphsMap_t &graph_map, std::istream &in, std::ostream &out);
  void inbound(const GraphsMap_t &graph_map, std::istream &in, std::ostream &out);
  void bind(GraphsMap_t &graph_map, std::istream &in);
  void cut(GraphsMap_t &graph_map, std::istream &in);
  void create(GraphsMap_t &graph_map, std::istream &in);
  void merge(GraphsMap_t &graph_map, std::istream &in);
  void extract(GraphsMap_t &graph_map, std::istream &in);
}

#endif
