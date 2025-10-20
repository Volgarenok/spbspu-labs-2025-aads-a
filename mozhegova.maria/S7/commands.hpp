#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include "graph.hpp"

namespace mozhegova
{
  using MapOfGraphs = BiTree< std::string, Graph >;
  void printGraphs(std::ostream & out, const MapOfGraphs & graphs);
  void printVertexes(std::istream & in, std::ostream & out, const MapOfGraphs & graphs);
  void printOutbounds(std::istream & in, std::ostream & out, const MapOfGraphs & graphs);
  void printInbounds(std::istream & in, std::ostream & out, const MapOfGraphs & graphs);
  void bind(std::istream & in, MapOfGraphs & graphs);
  void cut(std::istream & in, MapOfGraphs & graphs);
  void create(std::istream & in, MapOfGraphs & graphs);
  void merge(std::istream & in, MapOfGraphs & graphs);
  void extract(std::istream & in, MapOfGraphs & graphs);
}

#endif
