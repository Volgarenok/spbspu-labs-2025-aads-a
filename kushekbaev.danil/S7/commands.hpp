#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include "graph.hpp"

namespace kushekbaev
{
  using graphsTree = std::map< std::string, Graph >;

  void print_graphs(std::ostream& out, const graphsTree& graphs);
  void vertexes(std::istream& in, std::ostream& out, const graphsTree& graphs);
  void out_bound(std::istream& in, std::ostream& out, const graphsTree& graphs);
  void in_bound(std::istream& in, std::ostream& out, const graphsTree& graphs);
  void bind(std::istream& in, graphsTree& graphs);
  void cut(std::istream& in, graphsTree& graphs);
  void create(std::istream& in, graphsTree& graphs);
  void merge(std::istream& in, graphsTree& graphs);
  void extract(std::istream& in, graphsTree& graphs);
}

#endif
