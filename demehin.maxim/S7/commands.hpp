#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include <tree/tree.hpp>
#include "graph.hpp"

namespace demehin
{
  using MapOfGraphs = demehin::Tree< std::string, Graph >;

  void printGraphsNames(std::ostream&, const MapOfGraphs&);
  void printVertexesNames(std::ostream&, std::istream&, const MapOfGraphs&);
  void printOutbounds(std::ostream&, std::istream&, const MapOfGraphs&);
  void printInbounds(std::ostream&, std::istream&, const MapOfGraphs&);
  void bind(std::istream&, MapOfGraphs&);
  void cut(std::istream&, MapOfGraphs&);
  void create(std::istream&, MapOfGraphs&);
  void merge(std::istream&, MapOfGraphs&);
  void extract(std::istream&, MapOfGraphs&);
}

#endif
