#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include <string>
#include "tree/tree.hpp"

#include "graph.hpp"

namespace zholobov {

  using MapOfGraphs = Tree< std::string, Graph >;

  void inputGraphs(std::istream& in, MapOfGraphs& graphs);

  void cmdPrintGraphsNames(std::ostream& out, const MapOfGraphs& graphs);
  void cmdPrintVertexesNames(std::ostream& out, std::istream& in, const MapOfGraphs& graphs);
  void cmdPrintOutbounds(std::ostream& out, std::istream& in, const MapOfGraphs& graphs);
  void cmdPrintInbounds(std::ostream& out, std::istream& in, const MapOfGraphs& graphs);
  void cmdBind(std::istream& in, MapOfGraphs& graphs);
  void cmdCut(std::istream& in, MapOfGraphs& graphs);
  void cmdCreate(std::istream& in, MapOfGraphs& graphs);
  void cmdMerge(std::istream& in, MapOfGraphs& graphs);
  void cmdExtract(std::istream& in, MapOfGraphs& graphs);

}

#endif
