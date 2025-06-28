#include "graph.hpp"
#include <algorithm>

void duhanina::Graph::addVertex(const Vertex& v)
{
  vertices.insert(std::make_pair(v, true));
}

void duhanina::Graph::addEdge(const Vertex& from, const Vertex& to, Weight weight)
{
  addVertex(from);
  addVertex(to);
  Edge edge = std::make_pair(from, to);
  auto it = edges.find(edge);
  if (it != edges.end())
  {
    it->second.push_back(weight);
  }
  else
  {
    WeightsList weights;
    weights.push_back(weight);
    edges.insert(std::make_pair(edge, weights));
  }
}

bool duhanina::Graph::hasVertex(const Vertex& v) const
{
  return vertices.find(v) != vertices.cend();
}

bool duhanina::Graph::hasEdge(const Vertex& from, const Vertex& to, Weight weight) const
{
  Edge edge = std::make_pair(from, to);
  auto it = edges.find(edge);
  if (it != edges.cend())
  {
    for (auto wit = it->second.cbegin(); wit != it->second.cend(); ++wit)
    {
      if (*wit == weight)
      {
        return true;
      }
    }
  }
  return false;
}

void duhanina::Graph::removeEdge(const Vertex& from, const Vertex& to, Weight weight)
{
  Edge edge = std::make_pair(from, to);
  auto it = edges.find(edge);
  if (it != edges.end())
  {
    it->second.remove(weight);
    if (it->second.empty())
    {
      edges.erase(edge);
    }
  }
}

duhanina::Tree< std::string, duhanina::Graph::WeightsList, std::less< std::string > > duhanina::Graph::getOutbound(const Vertex& from) const
{
  Tree< Vertex, WeightsList, std::less< std::string > > result;
  for (auto eit = edges.cbegin(); eit != edges.cend(); ++eit)
  {
    if (eit->first.first == from)
    {
      WeightsList sorted = eit->second;
      result.insert(std::make_pair(eit->first.second, sorted));
    }
  }
  return result;
}

duhanina::Tree< std::string, duhanina::Graph::WeightsList, std::less< std::string > > duhanina::Graph::getInbound(const Vertex& to) const
{
  Tree< Vertex, WeightsList, std::less< std::string > > result;
  for (auto eit = edges.cbegin(); eit != edges.cend(); ++eit)
  {
    if (eit->first.second == to)
    {
      WeightsList sorted = eit->second;
      result.insert(std::make_pair(eit->first.first, sorted));
    }
  }
  return result;
}

duhanina::List< duhanina::Graph::Vertex > duhanina::Graph::getVertices() const
{
  List< Vertex > result;
  for (auto vit = vertices.cbegin(); vit != vertices.cend(); ++vit)
  {
    result.push_back(vit->first);
  }
  return result;
}

size_t duhanina::Graph::vertexCount() const
{
  return vertices.size();
}

const duhanina::HashTable< duhanina::Graph::Edge, duhanina::Graph::WeightsList, duhanina::EdgeHash >& duhanina::Graph::getAllEdges() const
{
  return edges;
}
