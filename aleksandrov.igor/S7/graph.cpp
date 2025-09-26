#include "graph.hpp"
#include <stdexcept>

namespace
{
  void sortWeights(aleksandrov::Vector< unsigned int >& weights)
  {
    for (size_t i = 1; i < weights.size(); ++i)
    {
      auto key = weights[i];
      size_t j = i;
      while (j > 0 && weights[j - 1] > key)
      {
        weights[j] = weights[j - 1];
        --j;
      }
      weights[j] = key;
    }
  }
}

void aleksandrov::Graph::bind(const VertexName& from, const VertexName& to, unsigned int w)
{
  vertices.insert({ from, true });
  vertices.insert({ to, true });
  VertexPair edgeKey = { from, to };
  edges[edgeKey].pushBack(w);
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
      if (std::next(it) != weights.end())
      {
        *it = weights.back();
      }
      weights.popBack();
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
      Weights weights;
      for (size_t i = 0; i < it->second.size(); ++i)
      {
        weights.pushBack(it->second[i]);
      }
      sortWeights(weights);
      result.insert({ it->first.second, weights });
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
      Weights weights;
      for (size_t i = 0; i < it->second.size(); ++i)
      {
        weights.pushBack(it->second[i]);
      }
      sortWeights(weights);
      result.insert({ it->first.first, weights });
    }
  }
  return result;
}

