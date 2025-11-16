#include "graph.hpp"
#include <algorithm>

void zholobov::Graph::addVertex(const Vertex& v)
{
  vertices.insert(std::make_pair(v, true));
}

void zholobov::Graph::addEdge(const Vertex& from, const Vertex& to, Weight weight)
{
  addVertex(from);
  addVertex(to);
  Edge edge = std::make_pair(from, to);
  auto it = edges.find(edge);
  if (it != edges.end()) {
    it->second.push_back(weight);
  } else {
    WeightsList weights;
    weights.push_back(weight);
    edges.insert(std::make_pair(edge, weights));
  }
}

bool zholobov::Graph::hasVertex(const Vertex& v) const
{
  return vertices.find(v) != vertices.cend();
}

bool zholobov::Graph::hasEdge(const Vertex& from, const Vertex& to, Weight weight) const
{
  Edge edge = std::make_pair(from, to);
  auto it = edges.find(edge);
  if (it != edges.cend()) {
    for (auto wit = it->second.cbegin(); wit != it->second.cend(); ++wit) {
      if (*wit == weight) {
        return true;
      }
    }
  }
  return false;
}

void zholobov::Graph::removeEdge(const Vertex& from, const Vertex& to, Weight weight)
{
  Edge edge = std::make_pair(from, to);
  auto it = edges.find(edge);
  if (it != edges.end()) {
    it->second.remove(weight);
    if (it->second.empty()) {
      edges.erase(edge);
    }
  }
}

zholobov::Tree< std::string, zholobov::Graph::WeightsList > zholobov::Graph::getOutbound(const Vertex& from) const
{
  Tree< Vertex, WeightsList > result;
  for (auto eit = edges.cbegin(); eit != edges.cend(); ++eit) {
    if (eit->first.first == from) {
      WeightsList sorted = eit->second;
      result.insert(std::make_pair(eit->first.second, sorted));
    }
  }
  return result;
}

zholobov::Tree< std::string, zholobov::Graph::WeightsList > zholobov::Graph::getInbound(const Vertex& to) const
{
  Tree< Vertex, WeightsList > result;
  for (auto eit = edges.cbegin(); eit != edges.cend(); ++eit) {
    if (eit->first.second == to) {
      WeightsList sorted = eit->second;
      result.insert(std::make_pair(eit->first.first, sorted));
    }
  }
  return result;
}

zholobov::Graph::Vertices zholobov::Graph::getVertices() const
{
  CircularFwdList< Vertex > result;
  for (auto vit = vertices.cbegin(); vit != vertices.cend(); ++vit) {
    result.push_back(vit->first);
  }
  return result;
}

size_t zholobov::Graph::vertexCount() const
{
  return vertices.size();
}

const zholobov::HashTable< zholobov::Graph::Edge, zholobov::Graph::WeightsList, zholobov::EdgeHash >&
zholobov::Graph::getAllEdges() const
{
  return edges;
}
