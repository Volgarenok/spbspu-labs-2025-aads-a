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
  const auto& vertices = graphIt->second.vertices;
  if (vertices.empty())
  {
    out << '\n';
  }
  for (auto it = vertices.cbegin(); it != vertices.cend(); ++it)
  {
    out << it->first << '\n';
  }
}

void aleksandrov::outBound(const Graphs& graphs, std::istream& in, std::ostream& out)
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
  const Graph& graph = graphIt->second;
  if (graph.vertices.find(vertexName) == graph.vertices.end())
  {
    throw std::logic_error("No such vertex in graph exists!");
  }
  Graph::Bounds outBounds = graph.getOutBounds(vertexName);
  if (outBounds.empty())
  {
    out << '\n';
    return;
  }
  for (auto it = outBounds.cbegin(); it != outBounds.cend(); ++it)
  {
    out << it->first;
    const auto& weights = it->second;
    for (size_t i = 0; i < weights.size(); ++i)
    {
      out << ' ' << weights[i];
    }
    out << '\n';
  }
}

void aleksandrov::inBound(const Graphs& graphs, std::istream& in, std::ostream& out)
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
  const Graph& graph = graphIt->second;
  if (graph.vertices.find(vertexName) == graph.vertices.end())
  {
    throw std::logic_error("No such vertex in graph exists!");
  }
  Graph::Bounds inBounds = graph.getInBounds(vertexName);
  if (inBounds.empty())
  {
    out << '\n';
    return;
  }
  for (auto it = inBounds.cbegin(); it != inBounds.cend(); ++it)
  {
    out << it->first;
    const auto& weights = it->second;
    for (size_t i = 0; i < weights.size(); ++i)
    {
      out << ' ' << weights[i];
    }
    out << '\n';
  }
}

void aleksandrov::bind(Graphs& graphs, std::istream& in)
{
  std::string graphName;
  std::string from;
  std::string to;
  unsigned int weight = 0;
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
  unsigned int weight = 0;
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

void aleksandrov::create(Graphs& graphs, std::istream& in)
{
  std::string newGraphName;
  if (!(in >> newGraphName))
  {
    throw std::logic_error("Incorrect graph name!");
  }
  if (graphs.find(newGraphName) != graphs.end())
  {
    throw std::logic_error("Graph " + newGraphName + " already exists!");
  }
  size_t verticesCount = 0;
  if (!(in >> verticesCount))
  {
    throw std::logic_error("Incorrect vertices!");
  }
  Graph newGraph;
  if (!verticesCount)
  {
    graphs.insert({ newGraphName, newGraph });
    return;
  }
  for (size_t i = 0; i < verticesCount; ++i)
  {
    std::string vertexName;
    if (!(in >> vertexName))
    {
      throw std::logic_error("Incorrect vertex name(s)!");
    }
    newGraph.vertices.insert({ vertexName, true });
  }
  graphs.insert({ newGraphName, newGraph });
}

void aleksandrov::merge(Graphs& graphs, std::istream& in)
{
  std::string newGraphName;
  std::string graphName1;
  std::string graphName2;
  if (!(in >> newGraphName >> graphName1 >> graphName2))
  {
    throw std::logic_error("Incorrect graph name(s)!");
  }
  if (graphs.find(newGraphName) != graphs.end())
  {
    throw std::logic_error("Graph " + newGraphName + " already exists!");
  }
  auto graphIt1 = graphs.find(graphName1);
  auto graphIt2 = graphs.find(graphName2);
  if (graphIt1 == graphs.end() || graphIt2 == graphs.end())
  {
    throw std::logic_error("Incorrect graph names!");
  }
  Graph newGraph;
  const Graph& graph1 = graphIt1->second;
  newGraph.vertices.insert(graph1.vertices.begin(), graph1.vertices.end());
  for (auto it = graph1.edges.cbegin(); it != graph1.edges.cend(); ++it)
  {
    newGraph.edges[it->first] = it->second;
  }
  const Graph& graph2 = graphIt2->second;
  newGraph.vertices.insert(graph2.vertices.begin(), graph2.vertices.end());
  for (auto it = graph2.edges.cbegin(); it != graph2.edges.cend(); ++it)
  {
    auto& weights = newGraph.edges[it->first];
    for (size_t i = 0; i < it->second.size(); ++i)
    {
      weights.pushBack(it->second[i]);
    }
  }
  graphs.insert({ newGraphName, newGraph });
}

void aleksandrov::extract(Graphs& graphs, std::istream& in)
{
  std::string newGraphName;
  std::string graphName;
  size_t verticesCount = 0;
  if (!(in >> newGraphName >> graphName >> verticesCount))
  {
    throw std::logic_error("Incorrect graph name(s) or description!");
  }
  Tree< std::string, bool > requestedVertices;
  for (size_t i = 0; i < verticesCount; ++i)
  {
    std::string vertexName;
    if (!(in >> vertexName))
    {
      throw std::logic_error("Incorrect vertex name(s)!");
    }
    requestedVertices.insert({ vertexName, true });
  }
  if (graphs.find(newGraphName) != graphs.end())
  {
    throw std::logic_error("Graph " + newGraphName + " already exists!");
  }
  auto graphIt = graphs.find(graphName);
  if (graphIt == graphs.end())
  {
    throw std::logic_error("Incorrect graph name!");
  }
  const Graph& graph = graphIt->second;
  for (auto it = requestedVertices.cbegin(); it != requestedVertices.cend(); ++it)
  {
    if (graph.vertices.find(it->first) == graph.vertices.end())
    {
      throw std::logic_error("Incorrect vertices!");
    }
  }
  Graph newGraph;
  for (auto it = requestedVertices.cbegin(); it != requestedVertices.cend(); ++it)
  {
    newGraph.vertices.insert(*it);
  }
  for (auto it = graph.edges.cbegin(); it != graph.edges.cend(); ++it)
  {
    const auto& from = it->first.first;
    const auto& to = it->first.second;
    if (requestedVertices.count(from) && requestedVertices.count(to))
    {
      newGraph.edges.insert({ it->first, it->second });
    }
  }
  graphs.insert({ newGraphName, newGraph });
}

