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
  auto graphIt = graphs.find(graphName);
  if (graphIt == graphs.end())
  {
    throw std::logic_error("No such graph exists!");
  }

  const Vertices& vertices = graphIt->second.vertices;
  if (vertices.empty())
  {
    out << '\n';
  }
  for (auto it = vertices.cbegin(); it != vertices.cend(); ++it)
  {
    out << it->first << '\n';
  }
}

void aleksandrov::outbound(const Graphs& graphs, std::istream& in, std::ostream& out)
{
  std::string graphName;
  std::string vertexName;
  if (!(in >> graphName >> vertexName))
  {
    throw std::logic_error("Incorrect graph or vertex name!");
  }
  auto graphIt = graphs.find(graphName);
  if (graphIt == graphs.end())
  {
    throw std::logic_error("No such graph exists!");
  }
  const Vertices& vertices = graphIt->second.vertices;
  auto vertexIt = vertices.find(vertexName);
  if (vertexIt == vertices.end())
  {
    throw std::logic_error("No such vertex in graph exists!");
  }

  const AdjacentyList& adjList = vertexIt->second.adjacentyList;
  if (adjList.empty())
  {
    out << '\n';
  }
  for (auto it = adjList.cbegin(); it != adjList.cend(); ++it)
  {
    out << it->dest << ' ' << it->weight << '\n';
  }
}

void aleksandrov::inbound(const Graphs& graphs, std::istream& in, std::ostream& out)
{
  std::string graphName;
  std::string vertexName;
  if (!(in >> graphName >> vertexName))
  {
    throw std::logic_error("Incorrect graph or vertex name!");
  }
  auto graphIt = graphs.find(graphName);
  if (graphIt == graphs.end())
  {
    throw std::logic_error("No such graph exists!");
  }
  const Vertices& vertices = graphIt->second.vertices;
  auto vertexIt = vertices.find(vertexName);
  if (vertexIt == vertices.end())
  {
    throw std::logic_error("No such vertex in graph exists!");
  }

  if (vertices.size() == 1)
  {
    out << '\n';
  }
  for (auto it = vertices.cbegin(); it != vertices.cend(); ++it)
  {
    const AdjacentyList& adjList = it->second.adjacentyList;
    for (size_t i = 0; i < adjList.size(); ++i)
    {
      if (adjList[i].dest == vertexName)
      {
        out << it->first << ' ' << adjList[i].weight << '\n';
      }
    }
  }
}

void aleksandrov::bind(Graphs& graphs, std::istream& in)
{
  std::string graphName;
  std::string from;
  std::string to;
  size_t weight = 0;
  if (!(in >> graphName >> from >> to >> weight))
  {
    throw std::logic_error("Incorrect graph name or description!");
  }
  auto graphIt = graphs.find(graphName);
  if (graphIt == graphs.end())
  {
    throw std::logic_error("No such graph exists!");
  }
  graphIt->second.bind(from, to, weight);
}

void aleksandrov::cut(Graphs& graphs, std::istream& in)
{
  std::string graphName;
  std::string from;
  std::string to;
  size_t weight = 0;
  if (!(in >> graphName >> from >> to >> weight))
  {
    throw std::logic_error("Incorrect graph name or description!");
  }
  auto graphIt = graphs.find(graphName);
  if (graphIt == graphs.end())
  {
    throw std::logic_error("No such graph exists!");
  }
  graphIt->second.cut(from, to, weight);
}

