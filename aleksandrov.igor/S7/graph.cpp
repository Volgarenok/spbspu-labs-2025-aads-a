#include "graph.hpp"
#include <stdexcept>

void aleksandrov::Graph::bind(const std::string& from, const std::string& to, size_t w)
{
  vertices.insert({ from, Vertex{} });
  vertices.insert({ to, Vertex{} });
  vertices[from].adjacentyList.push_back({ to, w });
}

void aleksandrov::Graph::cut(const std::string& from, const std::string& to, size_t w)
{
  auto fromIt = vertices.find(from);
  if (fromIt == vertices.end())
  {
    throw std::logic_error("No such vertex in graph " + name + " exists!");
  }
  auto edgeIt = findEdge(from, to, w);
  if (edgeIt != fromIt->second.adjacentyList.end())
  {
    fromIt->second.adjacentyList.erase(edgeIt);
  }
  else
  {
    throw std::logic_error("No such edge in graph " + name + " exists!");
  }
}

auto aleksandrov::Graph::findEdge(const std::string& from, const std::string& to, size_t w) -> AdjListIterator
{
  auto fromIt = vertices.find(from);
  if (fromIt == vertices.end())
  {
    return fromIt->second.adjacentyList.end();
  }
  AdjacentyList& adjList = fromIt->second.adjacentyList;
  for (auto it = adjList.begin(); it != adjList.end(); ++it)
  {
    if (it->dest == to && it->weight == w)
    {
      return it;
    }
  }
  return adjList.end();
}

