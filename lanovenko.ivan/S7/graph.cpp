#include "graph.hpp"

namespace
{
  void bubble_sort(std::vector< std::pair< std::string, std::vector< int > > >& vec)
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

void lanovenko::Graph::extract(const Graph& graph, const std::set< std::string >& extractVertexes)
{
  for (auto it = extractVertexes.cbegin(); it != extractVertexes.cend(); it++)
  {
    add_vertex(*it);
  }
  for (auto it = graph.edges.cbegin(); it != graph.edges.cend(); it++)
  {
    const auto vertexes = it->first;
    if (has_vertex(vertexes.first) && has_vertex(vertexes.second))
    {
      for (auto itWeight = it->second.cbegin(); itWeight != it->second.cend(); ++itWeight)
      {
        add_edge(vertexes.first, vertexes.second, *itWeight);
      }
    }
  }
}

void lanovenko::Graph::merge(const Graph& graph)
{
  for (auto it = graph.edges.cbegin(); it != graph.edges.cend(); it++)
  {
    for (auto itWeight = it->second.cbegin(); itWeight != it->second.cend(); ++itWeight)
    {
      add_edge(it->first.first, it->first.second, *itWeight);
    }
  }
  for (auto it = graph.vertexes.cbegin(); it != graph.vertexes.cend(); it++)
  {
    add_vertex(*it);
  }
}

void lanovenko::Graph::add_vertex(const std::string& v)
{
  vertexes.insert(v);
}

bool lanovenko::Graph::has_vertex(const std::string& vertex) const
{
  return vertexes.find(vertex) != vertexes.end();
}

bool lanovenko::Graph::has_edge(const std::string& v1, const std::string& v2, int weight) const
{
  auto it = edges.find({ v1, v2 });
  if (it == edges.cend())
  {
    return false;
  }
  for (int existWeight : it->second)
  {
    if (existWeight == weight)
    {
      return true;
    }
  }
  return false;
}

void lanovenko::Graph::add_edge(const std::string& v1, const std::string& v2, int weight)
{
  edges[{v1, v2}].emplace_back(weight);
  vertexes.insert(v1);
  vertexes.insert(v2);
}

void lanovenko::Graph::delete_edge(const std::string& v1, const std::string& v2, int weight)
{
  auto it = edges.find({ v1, v2 });
  if (it == edges.end())
  {
    throw std::logic_error("ERROR: there is no such edge");
  }
  auto& weights = it->second;
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

std::vector< std::string > lanovenko::Graph::get_vertexes() const
{
  return { vertexes.begin(), vertexes.end() };
}

std::vector< std::pair< std::string, std::vector< int > > > lanovenko::Graph::get_outbound(const std::string& vertex) const
{
  std::vector< std::pair< std::string, std::vector< int > > > result;
  for (auto it = edges.cbegin(); it != edges.cend(); it++)
  {
    if (it->first.first == vertex)
    {
      result.push_back({ it->first.second, edges.at({it->first.first, it->first.second}) });
    }
  }
  bubble_sort(result);
  return result;
}

std::vector< std::pair< std::string, std::vector< int > > > lanovenko::Graph::get_inbound(const std::string& vertex) const
{
  std::vector< std::pair< std::string, std::vector< int > > > result;
  for (auto it = edges.cbegin(); it != edges.cend(); it++)
  {
    if (it->first.second == vertex)
    {
      result.push_back({ it->first.first, edges.at({it->first.first, it->first.second}) });
    }
  }
  bubble_sort(result);
  return result;
}
