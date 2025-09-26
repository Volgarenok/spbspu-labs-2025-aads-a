#include "graph.hpp"
#include <algorithm>
#include <stdexcept>

void krylov::Graph::addVertex(const std::string & str)
{
  vertexes_.insert(std::make_pair(str, true));
}

void krylov::Graph::addEdge(std::string first, std::string second, size_t weigth)
{
  addVertex(first);
  addVertex(second);
  edges_[std::make_pair(first, second)][weigth]++;
}

void krylov::Graph::addEdges(const Graph& other)
{
  for (auto iter = other.edges_.cbegin(); iter != other.edges_.cend(); ++iter)
  {
    for (auto iter2 = iter->second.cbegin(); iter2 != iter->second.cend(); ++iter2)
    {
      for (size_t i = 0; i < iter2->second; ++i)
      {
        addEdge(iter->first.first, iter->first.second, iter2->first);
      }
    }
  }
}

bool krylov::Graph::removeEdge(const std::string& first, const std::string& second, size_t weigth)
{
  auto it = edges_.find({ first, second });
  if (it == edges_.end())
  {
    return false;
  }
  auto pos = it->second.find(weigth);
  if (pos == it->second.end())
  {
    return false;
  }
  it->second.erase(pos);
  if (it->second.empty())
  {
    edges_.erase(it);
  }
  return true;
}

std::map< std::string, bool > krylov::Graph::getVertexes() const
{
  return vertexes_;
}

std::unordered_map< std::pair< std::string, std::string >, std::map< size_t, size_t >, krylov::HashForPair  > krylov::Graph::getEdges() const
{
  return edges_;
}

bool krylov::Graph::hasVert(const std::string& str) const
{
  return vertexes_.find(str) != vertexes_.cend();
}

std::map< std::string, std::map< size_t, size_t > > krylov::Graph::getOutBound(const std::string& str) const
{
  if (!hasVert(str))
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  std::map< std::string, std::map< size_t, size_t > > temp;
  for (auto i = edges_.cbegin(); i != edges_.cend(); ++i)
  {
    const std::map< size_t, size_t >& weights = i->second;
    if (i->first.first == str)
    {
      temp[i->first.second] = weights;
    }
  }
  return temp;
}

std::map< std::string, std::map< size_t, size_t > > krylov::Graph::getInBound(const std::string& str) const
{
  if (!hasVert(str))
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  std::map< std::string, std::map< size_t, size_t > > temp;
  for (auto i = edges_.cbegin(); i != edges_.cend(); ++i)
  {
    const std::map< size_t, size_t >& weights = i->second;
    if (i->first.second == str)
    {
      temp[i->first.first] = weights;
    }
  }
  return temp;
}
