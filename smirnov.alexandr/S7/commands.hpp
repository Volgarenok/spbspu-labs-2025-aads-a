#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include <string>
#include "graphCollection.hpp"

namespace smirnov
{
  void graphs(const GraphCollection & graphs, std::ostream & out);
  void vertexes(const GraphCollection & graphs, std::istream & in, std::ostream & out);
  void outbound(const GraphCollection & graphs, std::istream & in, std::ostream & out);
  void inbound(const GraphCollection & graphs, std::istream & in, std::ostream & out);
  void bind(GraphCollection & graphs, std::istream & in);
  void cut(GraphCollection & graphs, std::istream & in);
  void create(GraphCollection & graphs, std::istream & in);
  void merge(GraphCollection & graphs, std::istream & in);
  void extract(GraphCollection & graphs, std::istream & in);
}
#endif
