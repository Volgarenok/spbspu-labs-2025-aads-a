#include "graph.hpp"
#include <stdexcept>

void aleksandrov::Graph::bind(const VertexName& from, const VertexName& to, unsigned int w)
{
  vertices.insert({ from, true });
  vertices.insert({ to, true });
  VertexPair edgeKey = { from, to };
  edges[edgeKey].push_back(w);
}

void aleksandrov::Graph::cut(const VertexName& from, const VertexName& to, unsigned int w)
{
  if (vertices.find(from) == vertices.end() || vertices.find(to) == vertices.end())
  {
    throw std::logic_error("One or both vertices do not exist!");
  }
  auto edgeIt = edges.find({ from, to });
  if (edgeIt == edges.end())
  {
    throw std::logic_error("No such edge exists!");
  }
  Weights& weights = edgeIt->second;
  for (auto it = weights.begin(); it != weights.end(); ++it)
  {
    if (*it == w)
    {
      weights.erase(it);
      if (weights.empty())
      {
        edges.erase(edgeIt);
      }
      return;
    }
  }
  throw std::logic_error("No such weight in edge exists!");
}

auto aleksandrov::Graph::getOutBounds(const VertexName& from) const -> Bounds
{
  Bounds result;
  for (auto it = edges.cbegin(); it != edges.cend(); ++it)
  {
    if (it->first.first == from)
    {
      auto beginIt = it->second.begin();
      auto endIt = it->second.end();
      result[it->first.second].insert(result[it->first.second].end(), beginIt, endIt);
    }
  }
  return result;
}

auto aleksandrov::Graph::getInBounds(const VertexName& to) const -> Bounds
{
  Bounds result;
  for (auto it = edges.cbegin(); it != edges.cend(); ++it)
  {
    if (it->first.second == to)
    {
      auto beginIt = it->second.begin();
      auto endIt = it->second.end();
      result[it->first.first].insert(result[it->first.first].end(), beginIt, endIt);
    }
  }
  return result;
}

