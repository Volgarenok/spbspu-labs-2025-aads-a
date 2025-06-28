#include "commands.hpp"
#include <cstddef>
#include <list>
#include <map>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "graph.hpp"

namespace
{
  void bound(std::ostream& out, std::istream& in, const kiselev::Graphs& graphs, const std::string& b)
  {
    std::string graph;
    std::string vertex;
    in >> graph >> vertex;
    auto graphIt = graphs.find(graph);
    if (graphIt == graphs.end())
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    std::set< std::string > vertexes = graphIt->second.getVertexes();
    if (vertexes.empty() && vertexes.find(vertex) == vertexes.end())
    {
      throw std::logic_error("INVALID COMMAND");
    }
    kiselev::Graph::MapHash bound;
    if (b == "out")
    {
      bound = graphIt->second.getOutBound(vertex);
    }
    else
    {
      bound = graphIt->second.getInBound(vertex);
    }
    if (bound.empty())
    {
      out << "\n";
      return;
    }
    for (auto it = bound.begin(); it != bound.end(); ++it)
    {
      out << it->first;
      for (auto weightIt = it->second.begin(); weightIt != it->second.end(); ++weightIt)
      {
        for (size_t i = 0; i < weightIt->second; ++i)
        {
          out << " " << weightIt->first;
        }
      }
      out << "\n";
    }
  }

  void addEdges(kiselev::Graph& newGraph, const kiselev::Graph::Edge& edges)
  {
    for (auto it = edges.cbegin(); it != edges.cend(); ++it)
    {
      for (auto weightIt = it->second.begin(); weightIt != it->second.end(); ++weightIt)
      {
        newGraph.addEdge(it->first.first, it->first.second, *weightIt);
      }
    }
  }
}

void kiselev::input(std::istream& in, Graphs& graphs)
{
  std::string graph;
  while (in >> graph)
  {
    size_t count;
    in >> count;
    Graph newGraph;
    for (size_t i = 0; i < count; i++)
    {
      std::string v1;
      std::string v2;
      unsigned int weight;
      in >> v1 >> v2 >> weight;
      if (in.fail())
      {
        throw std::logic_error("Incorrect parameter");
      }
      newGraph.addEdge(v1, v2, weight);
    }
    graphs[graph] = newGraph;
  }
}

void kiselev::graphs(std::ostream& out, const Graphs& graphs)
{
  if (graphs.empty())
  {
    out << "\n";
    return;
  }
  for (auto it = graphs.begin(); it != graphs.end(); ++it)
  {
    out << it->first << "\n";
  }
}

void kiselev::vertexes(std::ostream& out, std::istream& in, const Graphs& graphs)
{
  std::string graph;
  in >> graph;
  auto it = graphs.find(graph);
  if (it == graphs.end())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  std::set< std::string > vertexes = it->second.getVertexes();
  if (vertexes.empty())
  {
    out << "\n";
    return;
  }
  for (auto iter = vertexes.begin(); iter != vertexes.end(); ++iter)
  {
    out << *iter << "\n";
  }
}

void kiselev::outBound(std::ostream& out, std::istream& in, const Graphs& graphs)
{
  bound(out, in, graphs, "out");
}

void kiselev::inBound(std::ostream& out, std::istream& in, const Graphs& graphs)
{
  bound(out, in, graphs, "in");
}

void kiselev::bind(std::istream& in, Graphs& graphs)
{
  std::string graph;
  std::string v1;
  std::string v2;
  unsigned int weight;
  in >> graph >> v1 >> v2 >> weight;
  if (in.fail())
  {
    throw std::logic_error("Incorrect parameter");
  }
  auto it = graphs.find(graph);
  if (it == graphs.end())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  it->second.addEdge(v1, v2, weight);
}

void kiselev::cut(std::istream& in, Graphs& graphs)
{
  std::string graph;
  std::string v1;
  std::string v2;
  unsigned int weight;
  in >> graph >> v1 >> v2 >> weight;
  if (in.fail())
  {
    throw std::logic_error("Incorrect parameter");
  }
  auto graphIt = graphs.find(graph);
  if (graphIt == graphs.end() || !graphIt->second.removeEdge(v1, v2, weight))
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
}

void kiselev::create(std::istream& in, Graphs& graphs)
{
  std::string graph;
  in >> graph;
  if (graphs.find(graph) != graphs.end())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Graph newGraph;
  size_t count;
  in >> count;
  if (in.fail())
  {
    throw std::logic_error("Incorrect parameter");
  }
  for (size_t i = 0; i < count; ++i)
  {
    std::string vertex;
    in >> vertex;
    newGraph.addEdge(vertex, vertex, 0);
  }
  graphs[graph] = newGraph;
}

void kiselev::merge(std::istream& in, Graphs& graphs)
{
  std::string newGraph;
  std::string gr1;
  std::string gr2;
  in >> newGraph >> gr1 >> gr2;
  if (graphs.find(newGraph) != graphs.end() || graphs.find(gr1) == graphs.end() || graphs.find(gr2) == graphs.end())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Graph result;
  addEdges(result, graphs.at(gr1).getEdges());
  addEdges(result, graphs.at(gr2).getEdges());
  graphs[newGraph] = result;
}

void kiselev::extract(std::istream& in, Graphs& graphs)
{
  std::string newGraph;
  std::string graph;
  size_t count;
  in >> newGraph >> graph >> count;
  if (in.fail())
  {
    throw std::logic_error("Incorrect parameter");
  }
  if (graphs.find(newGraph) != graphs.end() || graphs.find(graph) == graphs.end())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  std::set< std::string > vertexes1;
  for (size_t i = 0; i < count; ++i)
  {
    std::string vertex;
    in >> vertex;
    vertexes1.insert(vertex);
  }
  std::set< std::string > vertexes2 = graphs.at(graph).getVertexes();
  for (auto vertexIt = vertexes1.begin(); vertexIt != vertexes1.end(); ++vertexIt)
  {
    if (vertexes2.find(*vertexIt) == vertexes2.end())
    {
      throw std::logic_error("INVALID COMMAND");
    }
  }
  Graph result;
  Graph::Edge edge = graphs.at(graph).getEdges();
  for (auto edgeIt = edge.begin(); edgeIt != edge.end(); ++edgeIt)
  {
    auto it1 = vertexes1.find(edgeIt->first.first);
    auto it2 = vertexes1.find(edgeIt->first.second);
    if (it1 != vertexes1.end() && it2 != vertexes1.end())
    {
      const std::list< unsigned int >& weights = edgeIt->second;
      for (auto weightIt = weights.begin(); weightIt != weights.end(); ++weightIt)
      {
        result.addEdge(edgeIt->first.first, edgeIt->first.second, *weightIt);
      }
    }
  }
  graphs[newGraph] = result;
}
