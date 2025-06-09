#include "graph.hpp"

void demehin::Graph::addVertex(const std::string& vrt) noexcept
{
  if (!hasVrt(vrt))
  {
    addEdge(vrt, vrt, 0);
  }
}

void demehin::Graph::addEdge(const std::string& from, const std::string& to, unsigned int weight)
{
  auto key = std::make_pair(from, to);
  edges[key].push_back(weight);
}

demehin::Tree< std::string, std::string > demehin::Graph::getVrts() const
{
  Tree< std::string, std::string > vrts;
  for (auto it = edges.cbegin(); it != edges.cend(); it++)
  {
    vrts[it->first.first];
    vrts[it->first.second];
  }
  return vrts;
}

bool demehin::Graph::hasVrt(const std::string& vrt) const
{
  for (auto it = edges.cbegin(); it != edges.cend(); it++)
  {
    if (it->first.first == vrt || it->first.second == vrt)
    {
      return true;
    }
  }
  return false;
}

demehin::Tree< std::string, demehin::Tree< unsigned int, size_t > > demehin::Graph::getOutbounds(const std::string& vrt) const
{
  Tree< std::string, Tree< unsigned int, size_t > > res;

  for (auto it = edges.cbegin(); it != edges.cend(); it++)
  {
    if (it->first.first == vrt)
    {
      std::string target = it->first.second;
      for (auto&& weight: it->second)
      {
        res[target][weight]++;
      }
    }
  }
  return res;
}

demehin::Tree< std::string, demehin::Tree< unsigned int, size_t > > demehin::Graph::getInbounds(const std::string& vrt) const
{
  Tree< std::string, Tree< unsigned int, size_t > > res;

  for (auto it = edges.cbegin(); it != edges.cend(); it++)
  {
    if (it->first.second == vrt)
    {
      std::string target = it->first.first;
      for (auto&& weight: it->second)
      {
        res[target][weight]++;
      }
    }
  }
  return res;
}

bool demehin::Graph::deleteEdge(const std::string& from, const std::string& to, unsigned int weight)
{
  auto key = std::make_pair(from, to);
  auto edgeIt = edges.find(key);

  if (edgeIt == edges.end())
  {
    return false;
  }

  auto& weights = edgeIt->second;
  bool isDeleted = false;
  for (auto it = weights.cbegin(); it != weights.cend();)
  {
    if (*it == weight)
    {
      it = weights.erase(it);
      isDeleted = true;
    }
    else
    {
      it++;
    }
  }

  return isDeleted;
}

const demehin::Graph::Edges& demehin::Graph::getEdges() const
{
  return edges;
}
