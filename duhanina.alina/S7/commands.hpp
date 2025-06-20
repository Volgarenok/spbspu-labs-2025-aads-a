#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <functional>
#include "graph.hpp"

namespace duhanina
{
  using MapOfGraphs = Tree< std::string, Graph, std::less< std::string > >;

  void inputGraphs(std::istream& in, MapOfGraphs& graphs);
  void printGraphsNames(std::ostream& out, const MapOfGraphs& graphs);
  void printVertexesNames(std::ostream& out, std::istream& in, const MapOfGraphs& graphs);
  void printOutbounds(std::ostream& out, std::istream& in, const MapOfGraphs& graphs);
  void printInbounds(std::ostream& out, std::istream& in, const MapOfGraphs& graphs);
  void bind(std::istream& in, MapOfGraphs& graphs);
  void cut(std::istream& in, MapOfGraphs& graphs);
  void create(std::istream& in, MapOfGraphs& graphs);
  void merge(std::istream& in, MapOfGraphs& graphs);
  void extract(std::istream& in, MapOfGraphs& graphs);
}

#endif
