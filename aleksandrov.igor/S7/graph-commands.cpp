#include "graph-commands.hpp"
#include <iostream>

void aleksandrov::graphs(const Graphs& graphs, std::ostream& out)
{
  if (graphs.empty())
  {
    out << '\n';
  }
  for (auto it = graphs.cbegin(); it != graphs.cend(); ++it)
  {
    out << it->first << '\n';
  }
}

void aleksandrov::vertexes(const Graphs& graphs, std::istream& in, std::ostream& out)
{
  std::string graphName;
  if (!(in >> graphName))
  {
    throw std::logic_error("Incorrect graph name!");
  }
  auto it = graphs.find(graphName);
  if (it == graphs.end())
  {
    throw std::logic_error("No such graph exists!");
  }

  const Vertices& vertices = it->second.vertices;
  if (vertices.empty())
  {
    out << '\n';
  }
  for (auto it = vertices.cbegin(); it != vertices.cend(); ++it)
  {
    out << it->first << '\n';
  }
}

