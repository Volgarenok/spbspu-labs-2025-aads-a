#include "graph.hpp"
#include <algorithm>
#include <stdexcept>

void krylov::Graph::addVertex(const std::string & str)
{
  vertexes_.insert(str);
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

std::set< std::string > krylov::Graph::getVertexes() const
{
  return vertexes_;
}

krylov::Graph::hashMapForEdges krylov::Graph::getEdges() const
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
  for (auto iter = edges_.cbegin(); iter != edges_.cend(); ++iter)
  {
    if (iter->first.first == str)
    {
      for (auto iter2 = iter->second.cbegin(); iter2 != iter->second.cend(); iter2++)
      {
        temp[iter->first.second][iter2->first] = iter2->second;
      }
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
  for (auto iter = edges_.cbegin(); iter != edges_.cend(); ++iter)
  {
    if (iter->first.second == str)
    {
      for (auto iter2 = iter->second.cbegin(); iter2 != iter->second.cend(); iter2++)
      {
        temp[iter->first.first][iter2->first] = iter2->second;
      }
    }
  }
  return temp;
}
