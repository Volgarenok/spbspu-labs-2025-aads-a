#include "graph.hpp"
#include <stdexcept>

namespace
{
  void bubbleSort(std::vector< std::pair< std::string, std::vector< int > > > & vec)
  {
    for (size_t i = 0; i < vec.size(); ++i)
    {
      for (size_t j = i + 1; j < vec.size(); ++j)
      {
        if (vec[j] < vec[i])
        {
          std::swap(vec[i], vec[j]);
        }
      }
    }
  }
}


void maslov::Graph::extract(const Graph & graph, const std::set< std::string > & extractVertexes)
{
  for (auto it = extractVertexes.cbegin(); it != extractVertexes.cend(); it++)
  {
    addVertex(*it);
  }
  for (auto it = graph.edges.cbegin(); it != graph.edges.cend(); it++)
  {
    const auto vertexes = it->first;
    if (hasVertex(vertexes.first) && hasVertex(vertexes.second))
    {
      for (auto itWeight = it->second.cbegin(); itWeight != it->second.cend(); ++itWeight)
      {
        addEdge(vertexes.first, vertexes.second, *itWeight);
      }
    }
  }
}

void maslov::Graph::merge(const Graph & graph)
{
  for (auto it = graph.edges.cbegin(); it != graph.edges.cend(); it++)
  {
    for (auto itWeight = it->second.cbegin(); itWeight != it->second.cend(); ++itWeight)
    {
      addEdge(it->first.first, it->first.second, *itWeight);
    }
  }
  for (auto it = graph.vertexes.cbegin(); it != graph.vertexes.cend(); it++)
  {
    addVertex(*it);
  }
}

void maslov::Graph::addVertex(const std::string & v)
{
  vertexes.insert(v);
}

bool maslov::Graph::hasVertex(const std::string & vertex) const
{
  return vertexes.find(vertex) != vertexes.end();
}

bool maslov::Graph::hasEdge(const std::string & v1, const std::string & v2, int weight) const
{
  auto it = edges.find({v1, v2});
  if (it == edges.cend())
  {
    return false;
  }
  for (int existWeight: it->second)
  {
    if (existWeight == weight)
    {
      return true;
    }
  }
  return false;
}

void maslov::Graph::addEdge(const std::string & v1, const std::string & v2, int weight)
{
  edges[{v1, v2}].emplace_back(weight);
  vertexes.insert(v1);
  vertexes.insert(v2);
}

void maslov::Graph::deleteEdge(const std::string & v1, const std::string & v2, int weight)
{
  auto it = edges.find({v1, v2});
  if (it == edges.end())
  {
    throw std::logic_error("ERROR: there is no such edge");
  }
  auto & weights = it->second;
  auto itWeight = weights.begin();
  bool deleted = false;
  while (itWeight != weights.end())
  {
    if (*itWeight == weight)
    {
      itWeight = weights.erase(itWeight);
      deleted = true;
    }
    else
    {
      ++itWeight;
    }
  }
  if (!deleted)
  {
    throw std::logic_error("ERROR: there is no such weight");
  }
  if (weights.empty())
  {
    edges.erase(it);
  }
}

std::vector< std::string > maslov::Graph::getVertexes() const
{
  return {vertexes.begin(), vertexes.end()};
}

std::vector< std::pair< std::string, std::vector< int > > > maslov::Graph::getOutbound(const std::string & vertex) const
{
  std::vector< std::pair< std::string, std::vector< int > > > result;
  for (auto it = edges.cbegin(); it != edges.cend(); it++)
  {
    if (it->first.first == vertex)
    {
      result.push_back({it->first.second, edges.at({it->first.first, it->first.second})});
    }
  }
  bubbleSort(result);
  return result;
}

std::vector< std::pair< std::string, std::vector< int > > > maslov::Graph::getInbound(const std::string & vertex) const
{
  std::vector< std::pair< std::string, std::vector< int > > > result;
  for (auto it = edges.cbegin(); it != edges.cend(); it++)
  {
    if (it->first.second == vertex)
    {
      result.push_back({it->first.first, edges.at({it->first.first, it->first.second})});
    }
  }
  bubbleSort(result);
  return result;
}
