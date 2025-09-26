#include "graph.hpp"

void averenkov::Graph::addEdge(const std::string& from, const std::string& to, size_t weight)
{
  vertices.insert({from, true});
  vertices.insert({to, true});
  auto toMapIt = edges.find(from);
  if (toMapIt == edges.end())
  {
    HashTable< std::string, Array< size_t > > newMap;
    Array< size_t > weights;
    weights.push_back(weight);
    newMap.insert({to, weights});
    edges.insert({from, newMap});
  }
  else
  {
    auto weightsIt = toMapIt->value.find(to);
    if (weightsIt == toMapIt->value.end())
    {
      Array< size_t > weights;
      weights.push_back(weight);
      toMapIt->value.insert({to, weights});
    }
    else
    {
      weightsIt->value.push_back(weight);
    }
  }
}

void averenkov::Graph::addVertex(const std::string& vertex)
{
  if (vertices.find(vertex) == vertices.end())
  {
    vertices.insert({vertex, true});
  }
}

