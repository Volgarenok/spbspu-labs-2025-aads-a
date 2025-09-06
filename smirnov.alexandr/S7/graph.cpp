#include "graph.hpp"
#include <algorithm>

smirnov::Graph::Graph(const std::string & name):
  name_(name)
{}

const std::string & smirnov::Graph::getName() const noexcept
{
  return name_;
}

bool smirnov::Graph::hasVertex(const std::string & vertex) const
{
  return edges_.find(vertex) != edges_.cend();
}

bool smirnov::Graph::addVertex(const std::string & vertex)
{
  if (edges_.find(vertex) != edges_.end())
  {
    return false;
  }
  edges_.insert(vertex, HashTable< std::string, std::vector< size_t > >());
  return true;
}

bool smirnov::Graph::addEdge(const std::string & from, const std::string & to, size_t weight)
{
  addVertex(from);
  addVertex(to);
  auto fromIt = edges_.find(from);
  if (fromIt == edges_.end())
  {
    return false;
  }
  auto & toTable = const_cast< HashTable< std::string, std::vector< size_t > > & >(fromIt->second);
  auto toIt = toTable.find(to);
  if (toIt == toTable.end())
  {
    std::vector< size_t > weights;
    weights.push_back(weight);
    toTable.insert(to, weights);
  }
  else
  {
    auto & weights = const_cast< std::vector< size_t > & >(toIt->second);
    weights.push_back(weight);
  }
  return true;
}

bool smirnov::Graph::removeEdge(const std::string & from, const std::string & to, size_t weight)
{
  auto fromIt = edges_.find(from);
  if (fromIt == edges_.end())
  {
    return false;
  }
  auto & toTable = const_cast< smirnov::HashTable< std::string, std::vector< size_t > > & >(fromIt->second);
  auto toIt = toTable.find(to);
  if (toIt == toTable.end())
  {
    return false;
  }
  auto & weights = const_cast< std::vector< size_t > & >(toIt->second);
  for (size_t i = 0; i < weights.size(); ++i)
  {
    if (weights[i] == weight)
    {
      weights.erase(weights.begin() + i);
      if (weights.empty())
      {
        toTable.erase(to);
      }
      return true;
    }
  }
  return false;
}

std::vector< std::string > smirnov::Graph::getVertices() const
{
  std::vector< std::string > vertices;
  for (auto it = edges_.cbegin(); it != edges_.cend(); ++it)
  {
    vertices.push_back(it->first);
  }
  std::sort(vertices.begin(), vertices.end());
  return vertices;
}

std::vector< std::pair< std::string, size_t > > smirnov::Graph::getOutboundEdges(const std::string & vertex) const
{
  std::vector< std::pair< std::string, size_t > > result;
  auto vertexIt = edges_.find(vertex);
  if (vertexIt == edges_.cend())
  {
    return result;
  }
  const auto & outEdges = vertexIt->second;
  for (auto it = outEdges.cbegin(); it != outEdges.cend(); ++it)
  {
    const std::string & target = it->first;
    const std::vector< size_t > & weights = it->second;
    for (size_t i = 0; i < weights.size(); ++i)
    {
      result.push_back(std::make_pair(target, weights[i]));
    }
  }
  for (size_t i = 0; i < result.size(); ++i)
  {
    for (size_t j = i + 1; j < result.size(); ++j)
    {
      if (result[j].first < result[i].first || (result[j].first == result[i].first && result[j].second < result[i].second))
      {
        std::pair< std::string, size_t > tmp = result[i];
        result[i] = result[j];
        result[j] = tmp;
      }
    }
  }
  return result;
}

std::vector< std::pair< std::string, size_t > > smirnov::Graph::getInboundEdges(const std::string & vertex) const
{
  std::vector< std::pair< std::string, size_t > > result;
  for (auto fromIt = edges_.cbegin(); fromIt != edges_.cend(); ++fromIt)
  {
    const std::string & source = fromIt->first;
    const auto & outEdges = fromIt->second;
    auto toIt = outEdges.find(vertex);
    if (toIt != outEdges.cend())
    {
      const std::vector< size_t > & weights = toIt->second;
      for (size_t i = 0; i < weights.size(); ++i)
      {
        result.push_back(std::make_pair(source, weights[i]));
      }
    }
  }
  for (size_t i = 0; i < result.size(); ++i)
  {
    for (size_t j = i + 1; j < result.size(); ++j)
    {
      if (result[j].first < result[i].first || (result[j].first == result[i].first && result[j].second < result[i].second))
      {
        std::pair< std::string, size_t > tmp = result[i];
        result[i] = result[j];
        result[j] = tmp;
      }
    }
  }
  return result;
}
