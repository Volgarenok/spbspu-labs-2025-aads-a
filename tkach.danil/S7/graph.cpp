#include "graph.hpp"

namespace tkach
{
  bool Graph::removeEdge(const std::string& vert1, const std::string& vert2, size_t weight)
  {
    auto edge_key = std::make_pair(vert1, vert2);
    auto it = edges_.find(edge_key);
    if (it == edges_.end())
    {
      return false;
    }
    auto it2 = it->second.find(weight);
    if (it2 == it->second.end())
    {
      return  false;
    }
    it2->second--;
    if (it2->second == 0)
    {
      it->second.erase(it2);
    }
    return true;
  }

  void Graph::addEdge(const std::string& vert1, const std::string& vert2, size_t weight)
  {
    edges_[std::make_pair(vert1, vert2)][weight]++;
  }

  AvlTree< std::string, bool > Graph::getAllVert() const
  {
    AvlTree< std::string, bool > tree_of_verts;
    for (auto it = edges_.cbegin(); it != edges_.cend(); ++it)
    {
      tree_of_verts[it->first.first];
      tree_of_verts[it->first.second];
    }
    return tree_of_verts;
  }

  AvlTree< std::string, AvlTree< size_t, size_t > > Graph::getOutBound(const std::string& vert) const
  {
    AvlTree< std::string, AvlTree< size_t, size_t > > temp;
    for (auto it = edges_.cbegin(); it != edges_.cend(); ++it)
    {
      if (it->first.first == vert)
      {
        for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++)
        {
          temp[it->first.second][it2->first] = it2->second;
        }
      }
    }
    return temp;
  }

  AvlTree< std::string, AvlTree< size_t, size_t > > Graph::getInBound(const std::string& vert) const
  {
    AvlTree< std::string, AvlTree< size_t, size_t > > temp;
    for (auto it = edges_.cbegin(); it != edges_.cend(); ++it)
    {
      if (it->first.second == vert)
      {
        for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++)
        {
          temp[it->first.first][it2->first] = it2->second;
        }
      }
    }
    return temp;
  }

  void Graph::addEdges(const Graph& other)
  {
    for (auto it = other.edges_.cbegin(); it != other.edges_.cend(); ++it)
    {
      for (auto it2 = it->second.cbegin(); it2 != it->second.cend(); ++it2)
      {
        for (size_t i = 0; i < it2->second; ++i)
        {
          addEdge(it->first.first, it->first.second, it2->first);
        }
      }
    }
  }

  bool Graph::hasVert(const std::string& vert_name) const
  {
    for (auto it = edges_.cbegin(); it != edges_.cend(); ++it)
    {
      if (it->first.first == vert_name || it->first.second == vert_name)
      {
        return true;
      }
    }
    return false;
  }

  HashTable< std::pair< std::string, std::string >, AvlTree< size_t, size_t >, HashPair > Graph::gerGraphEdges() const
  {
    return edges_;
  }
}
