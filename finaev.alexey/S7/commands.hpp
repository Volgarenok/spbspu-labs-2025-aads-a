#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include <AVLtree.hpp>
#include "graph.hpp"

namespace finaev
{
  using graphsTree = AVLtree< std::string, Graph >;

  void printGraphs(std::ostream& out, const graphsTree& graphs);
  void vertexes(std::istream& in, std::ostream& out, const graphsTree& graphs);
  void outBound(std::istream& in, std::ostream& out, const graphsTree& graphs);
  void inBound(std::istream& in, std::ostream& out, const graphsTree& graphs);
  void bind(std::istream& in, graphsTree& graphs);
  void cut(std::istream& in, graphsTree& graphs);
  void create(std::istream& in, graphsTree& graphs);
  void merge(std::istream& in, graphsTree& graphs);
  void extract(std::istream& in, graphsTree& graphs);

  AVLtree< std::string, std::function< void() > > createCommandsHandler(std::istream&, std::ostream&, graphsTree&);
}

#endif
