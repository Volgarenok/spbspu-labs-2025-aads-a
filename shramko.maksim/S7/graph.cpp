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

void shramko::Graph::extract(const Graph& graph, const std::set< std::string >& extractVertexes)
{
  Graph temp;
  for (auto it = extractVertexes.cbegin(); it != extractVertexes.cend(); ++it)
  {
    temp.add_vertex(*it);
  }

  for (auto it = graph.edges.cbegin(); it != graph.edges.cend(); ++it)
  {
    const auto vertexes = it->first;

    if (temp.has_vertex(vertexes.first) && temp.has_vertex(vertexes.second))
    {
      for (auto itWeight = it->second.cbegin(); itWeight != it->second.cend(); ++itWeight)
      {
        temp.add_edge(vertexes.first, vertexes.second, *itWeight);
      }
    }
  }
  *this = std::move(temp);
}

void shramko::Graph::merge(const Graph& graph)
{
  Graph temp(*this);
  for (auto it = graph.vertexes.cbegin(); it != graph.vertexes.cend(); ++it)
  {
    temp.add_vertex(*it);
  }

  for (auto it = graph.edges.cbegin(); it != graph.edges.cend(); ++it)
  {
    for (auto itWeight = it->second.cbegin(); itWeight != it->second.cend(); ++itWeight)
    {
      temp.add_edge(it->first.first, it->first.second, *itWeight);
    }
  }
  *this = std::move(temp);
}

void shramko::Graph::add_vertex(const std::string& v)
{
  vertexes.insert(v);
}

bool shramko::Graph::has_vertex(const std::string& vertex) const
{
  return vertexes.find(vertex) != vertexes.end();
}

bool shramko::Graph::has_edge(const std::string& v1, const std::string& v2, int weight) const
{
  auto it = edges.find({v1, v2});

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

void shramko::Graph::add_edge(const std::string& v1, const std::string& v2, int weight)
{
  Graph temp(*this);
  temp.edges[{v1, v2}].push_back(weight);
  temp.vertexes.insert(v1);
  temp.vertexes.insert(v2);
  *this = std::move(temp);
}

void shramko::Graph::delete_edge(const std::string& v1, const std::string& v2, int weight)
{
  Graph temp(*this);
  auto it = temp.edges.find({v1, v2});

  if (it == temp.edges.end())
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
    temp.edges.erase(it);
  }
  *this = std::move(temp);
}

std::vector< std::string > shramko::Graph::get_vertexes() const
{
  return {vertexes.begin(), vertexes.end()};
}

std::vector< std::pair< std::string, std::vector< int > > > shramko::Graph::get_outbound(const std::string& vertex) const
{
  std::vector< std::pair< std::string, std::vector< int > > > result;

  for (auto it = edges.cbegin(); it != edges.cend(); ++it)
  {
    if (it->first.first == vertex)
    {
      result.push_back({it->first.second, it->second});
    }
  }

  bubble_sort(result);

  return result;
}

std::vector< std::pair< std::string, std::vector< int > > > shramko::Graph::get_inbound(const std::string& vertex) const
{
  std::vector< std::pair< std::string, std::vector< int > > > result;

  for (auto it = edges.cbegin(); it != edges.cend(); ++it)
  {
    if (it->first.second == vertex)
    {
      result.push_back({it->first.first, it->second});
    }
  }

  bubble_sort(result);

  return result;
}
