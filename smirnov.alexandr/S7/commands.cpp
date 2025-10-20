#include "commands.hpp"
#include <limits>
#include <algorithm>
#include <stdexcept>

void smirnov::graphs(const GraphCollection & graphs, std::ostream & out)
{
  auto names = graphs.listGraphs();
  if (names.empty())
  {
    out << "\n";
    return;
  }
  out << names[0];
  for (size_t i = 1; i < names.size(); ++i)
  {
    out << "\n" << names[i];
  }
  out << "\n";
}

void smirnov::vertexes(const GraphCollection & graphs, std::istream & in, std::ostream & out)
{
  std::string graphName;
  if (!(in >> graphName))
  {
    throw std::invalid_argument("Invalid command");
  }
  const Graph * graph = graphs.getGraph(graphName);
  if (!graph)
  {
    throw std::invalid_argument("Invalid command");
  }
  auto vertices = graph->getVertices();
  if (vertices.empty())
  {
    out << "\n";
    return;
  }
  out << vertices[0];
  for (size_t i = 1; i < vertices.size(); ++i)
  {
    out << "\n" << vertices[i];
  }
  out << "\n";
}

void smirnov::outbound(const GraphCollection & graphs, std::istream & in, std::ostream & out)
{
  std::string graphName, vertexName;
  if (!(in >> graphName >> vertexName))
  {
    throw std::invalid_argument("Invalid command\n");
  }
  const Graph * graph = graphs.getGraph(graphName);
  if (!graph || !graph->hasVertex(vertexName))
  {
    throw std::invalid_argument("Invalid command\n");
  }
  std::vector< std::pair< std::string, size_t > > edges = graph->getOutboundEdges(vertexName);
  if (edges.empty())
  {
    out << "\n";
    return;
  }
  for (size_t i = 0; i < edges.size(); ++i)
  {
    for (size_t j = i + 1; j < edges.size(); ++j)
    {
      if (edges[j].first < edges[i].first || (edges[j].first == edges[i].first && edges[j].second < edges[i].second))
      {
        std::pair< std::string, size_t> tmp = edges[i];
        edges[i] = edges[j];
        edges[j] = tmp;
      }
    }
  }
  size_t i = 0;
  while (i < edges.size())
  {
    out << edges[i].first;
    std::string current = edges[i].first;
    while (i < edges.size() && edges[i].first == current)
    {
      out << " " << edges[i].second;
      ++i;
    }
    out << "\n";
  }
}

void smirnov::inbound(const GraphCollection & graphs, std::istream & in, std::ostream & out)
{
  std::string graphName, vertexName;
  if (!(in >> graphName >> vertexName))
  {
    throw std::invalid_argument("Invalid command\n");
  }
  const Graph * graph = graphs.getGraph(graphName);
  if (!graph || !graph->hasVertex(vertexName))
  {
    throw std::invalid_argument("Invalid command\n");
  }
  std::vector< std::pair< std::string, size_t > > edges = graph->getInboundEdges(vertexName);
  if (edges.empty())
  {
    out << "\n";
    return;
  }
  for (size_t i = 0; i < edges.size(); ++i)
  {
    for (size_t j = i + 1; j < edges.size(); ++j)
    {
      if (edges[j].first < edges[i].first || (edges[j].first == edges[i].first && edges[j].second < edges[i].second))
      {
        std::pair< std::string, size_t > tmp = edges[i];
        edges[i] = edges[j];
        edges[j] = tmp;
      }
    }
  }
  size_t i = 0;
  while (i < edges.size())
  {
    out << edges[i].first;
    std::string current = edges[i].first;
    while (i < edges.size() && edges[i].first == current)
    {
      out << " " << edges[i].second;
      ++i;
    }
    out << "\n";
  }
}

void smirnov::bind(GraphCollection & graphs, std::istream & in)
{
  std::string graphName, from, to;
  unsigned weight;
  if (!(in >> graphName >> from >> to >> weight))
  {
    throw std::invalid_argument("Invalid command");
  }
  Graph * graph = graphs.getGraph(graphName);
  if (!graph)
  {
    throw std::invalid_argument("Invalid command");
  }
  graph->addEdge(from, to, weight);
}

void smirnov::cut(GraphCollection & graphs, std::istream & in)
{
  std::string graphName, from, to;
  unsigned weight;
  if (!(in >> graphName >> from >> to >> weight))
  {
    throw std::invalid_argument("Invalid command");
  }
  Graph * graph = graphs.getGraph(graphName);
  if (!graph)
  {
    throw std::invalid_argument("Invalid command");
  }
  if (!graph->removeEdge(from, to, weight))
  {
    throw std::invalid_argument("Invalid command");
  }
}

void smirnov::create(GraphCollection & graphs, std::istream & in)
{
  std::string graphName;
  if (!(in >> graphName))
  {
    throw std::invalid_argument("Invalid command");
  }
  if (!graphs.addGraph(graphName))
  {
    throw std::invalid_argument("Invalid command");
  }
  Graph * graph = graphs.getGraph(graphName);
  if (!graph)
  {
    throw std::invalid_argument("Invalid command");
  }
  size_t count;
  if (!(in >> count))
  {
    throw std::invalid_argument("Invalid command");
  }
  for (size_t i = 0; i < count; ++i)
  {
    std::string vertex;
    if (!(in >> vertex))
    {
      throw std::invalid_argument("Invalid command");
    }
    graph->addVertex(vertex);
  }
}
void smirnov::merge(GraphCollection & graphs, std::istream & in)
{
  std::string newName, name1, name2;
  if (!(in >> newName >> name1 >> name2))
  {
    throw std::invalid_argument("Invalid command");
  }
  if (graphs.getGraph(newName) != nullptr)
  {
    throw std::invalid_argument("Invalid command");
  }
  Graph * graph1 = graphs.getGraph(name1);
  Graph * graph2 = graphs.getGraph(name2);
  if (!graph1 || !graph2)
  {
    throw std::invalid_argument("Invalid command");
  }
  Graph mergedGraph(newName);
  auto vertices1 = graph1->getVertices();
  for (size_t i = 0; i < vertices1.size(); ++i)
  {
    mergedGraph.addVertex(vertices1[i]);
  }
  for (size_t i = 0; i < vertices1.size(); ++i)
  {
    auto edges = graph1->getOutboundEdges(vertices1[i]);
    for (size_t j = 0; j < edges.size(); ++j)
    {
      mergedGraph.addEdge(vertices1[i], edges[j].first, edges[j].second);
    }
  }
  auto vertices2 = graph2->getVertices();
  for (size_t i = 0; i < vertices2.size(); ++i)
  {
    if (!mergedGraph.hasVertex(vertices2[i]))
    {
      mergedGraph.addVertex(vertices2[i]);
    }
  }
  for (size_t i = 0; i < vertices2.size(); ++i)
  {
    auto edges = graph2->getOutboundEdges(vertices2[i]);
    for (size_t j = 0; j < edges.size(); ++j)
    {
      mergedGraph.addEdge(vertices2[i], edges[j].first, edges[j].second);
    }
  }
  graphs.addGraph(newName);
  Graph * added = graphs.getGraph(newName);
  if (added)
  {
    *added = mergedGraph;
  }
}

void smirnov::extract(GraphCollection & graphs, std::istream & in)
{
  std::string newName, oldName;
  size_t count;
  if (!(in >> newName >> oldName >> count))
  {
    throw std::invalid_argument("Invalid command");
  }
  if (graphs.getGraph(newName) != nullptr)
  {
    throw std::invalid_argument("Invalid command");
  }
  const Graph * oldGraph = graphs.getGraph(oldName);
  if (!oldGraph)
  {
    throw std::invalid_argument("Invalid command");
  }
  std::vector<std::string> vertices;
  for (size_t i = 0; i < count; ++i)
  {
    std::string v;
    if (!(in >> v))
    {
      throw std::invalid_argument("Invalid command");
    }
    if (!oldGraph->hasVertex(v))
    {
      throw std::invalid_argument("Invalid command");
    }
    vertices.push_back(v);
  }
  Graph newGraph(newName);
  for (size_t i = 0; i < vertices.size(); ++i)
  {
    newGraph.addVertex(vertices[i]);
  }
  for (size_t i = 0; i < vertices.size(); ++i)
  {
    const std::string & fromVertex = vertices[i];
    auto outEdges = oldGraph->getOutboundEdges(fromVertex);
    for (size_t j = 0; j < outEdges.size(); ++j)
    {
      const std::string & toVertex = outEdges[j].first;
      unsigned edgeCost = outEdges[j].second;
      bool vertexExists = false;
      for (size_t k = 0; k < vertices.size(); ++k)
      {
        if (vertices[k] == toVertex)
        {
          vertexExists = true;
          break;
        }
      }
      if (vertexExists)
      {
        newGraph.addEdge(fromVertex, toVertex, edgeCost);
      }
    }
  }
  graphs.addGraph(newName);
  Graph * added = graphs.getGraph(newName);
  if (added)
  {
    *added = newGraph;
  }
}
