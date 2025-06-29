#include "graph.hpp"

namespace mozhegova
{
  void Graph::addEdge(const std::string & v1, const std::string & v2, size_t weight)
  {
    addVertex(v1);
    addVertex(v2);
    auto pairVert = std::make_pair(v1, v2);
    edges[pairVert][weight]++;
  }

  void Graph::addVertex(const std::string & v)
  {
    vertexes.insert(std::make_pair(v, true));
  }

  bool Graph::hasVert(const std::string & v) const
  {
    return vertexes.find(v) != vertexes.cend();
  }

  BiTree< std::string, bool > Graph::getVertexes() const
  {
    return vertexes;
  }

  HashTable< std::pair< std::string, std::string >, BiTree< size_t, size_t >, HashPair > Graph::getEdges() const
  {
    return edges;
  }

  BiTree< std::string, BiTree< size_t, size_t > > Graph::getOutBounds(const std::string & v) const
  {
    if (!hasVert(v))
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    BiTree< std::string, BiTree< size_t, size_t > > temp;
    for (auto cit = edges.cbegin(); cit != edges.cend(); ++cit)
    {
      if (cit->first.first == v)
      {
        std::string v2 = cit->first.second;
        for (auto cit2 = cit->second.cbegin(); cit2 != cit->second.cend(); ++cit2)
        {
          temp[v2][cit2->first] = cit2->second;
        }
      }
    }
    return temp;
  }

  BiTree< std::string, BiTree< size_t, size_t > > Graph::getInBounds(const std::string & v) const
  {
    if (!hasVert(v))
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    BiTree< std::string, BiTree< size_t, size_t > > temp;
    for (auto cit = edges.cbegin(); cit != edges.cend(); ++cit)
    {
      if (cit->first.second == v)
      {
        std::string v2 = cit->first.first;
        for (auto cit2 = cit->second.cbegin(); cit2 != cit->second.cend(); ++cit2)
        {
          temp[v2][cit2->first] = cit2->second;
        }
      }
    }
    return temp;
  }

  bool Graph::removeEdge(const std::string & v1, const std::string & v2, size_t weigh)
  {
    auto pairVert = std::make_pair(v1, v2);
    auto it = edges.find(pairVert);
    if (it != edges.end())
    {
      auto it2 = it->second.find(weigh);
      if (it2 != it->second.end())
      {
        it2->second--;
        if (it2->second == 0)
        {
          it->second.erase(it2);
        }
        return true;
      }
    }
    return false;
  }

  void Graph::addEdges(const Graph & graph)
  {
    for (auto cit = graph.edges.cbegin(); cit != graph.edges.cend(); ++cit)
    {
      for (auto cit2 = cit->second.cbegin(); cit2 != cit->second.cend(); ++cit2)
      {
        for (size_t i = 0; i < cit2->second; ++i)
        {
          addEdge(cit->first.first, cit->first.second, cit2->first);
        }
      }
    }
  }
}
