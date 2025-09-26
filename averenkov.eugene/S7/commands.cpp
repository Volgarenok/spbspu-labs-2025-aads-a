#include "commands.hpp"
#include <algorithm>
#include <cctype>
#include <limits>

namespace
{
  void readVertices(std::istream& in, size_t count, averenkov::Array< std::string >& vertices)
  {
    std::string vertex;
    for (size_t i = 0; i < count; ++i)
    {
      if (!(in >> vertex))
      {
        throw std::invalid_argument("Not enough vertices");
      }
      vertices.push_back(vertex);
    }
  }
}

void averenkov::loadGraphsFromFile(Tree< std::string, Graph >& graphs, std::istream& in)
{
  std::string name;
  size_t edgeCount = 0;
  while (in >> name >> edgeCount)
  {
    Graph currentGraph;
    for (size_t i = 0; i < edgeCount; ++i)
    {
      std::string from;
      std::string to;
      size_t weight;
      if (!(in >> from >> to >> weight))
      {
        throw std::runtime_error("Invalid input format");
      }
      currentGraph.addEdge(from, to, weight);
    }
    graphs[name] = currentGraph;
  }
}

void averenkov::printGraphs(std::ostream& out, const Tree< std::string, Graph >& graphs)
{
  Array< std::string > graphNames;
  for (auto it = graphs.begin(); it != graphs.end(); ++it)
  {
    graphNames.push_back(it->first);
  }
  for (size_t i = 0; i < graphNames.size(); ++i)
  {
    for (size_t j = i + 1; j < graphNames.size(); ++j)
    {
      if (graphNames[j] < graphNames[i])
      {
        std::swap(graphNames[i], graphNames[j]);
      }
    }
  }
  if (graphNames.empty())
  {
    out << "\n";
    return;
  }
  for (size_t i = 0; i < graphNames.size(); ++i)
  {
    out << graphNames[i] << "\n";
  }
}

void averenkov::printVertices(std::ostream& out, std::istream& in, const Tree< std::string, Graph >& graphs)
{
  std::string graphName;
  in >> graphName;
  auto it = graphs.find(graphName);
  if (it == graphs.end())
  {
    throw std::invalid_argument("Invalid command");
  }
  Array< std::string > vertexNames;
  for (auto vit = it->second.vertices.begin(); vit != it->second.vertices.end(); ++vit)
  {
    vertexNames.push_back(vit->key);
  }
  for (size_t i = 0; i < vertexNames.size(); ++i)
  {
    size_t minIndex = i;
    for (size_t j = i + 1; j < vertexNames.size(); ++j)
    {
      if (vertexNames[j] < vertexNames[minIndex])
      {
        minIndex = j;
      }
    }
    if (minIndex != i)
    {
      std::swap(vertexNames[i], vertexNames[minIndex]);
    }
  }
  if (vertexNames.empty())
  {
    out << "\n";
    return;
  }
  for (size_t i = 0; i < vertexNames.size(); ++i)
  {
    out << vertexNames[i] << "\n";
  }
}

void averenkov::printOutbound(std::ostream& out, std::istream& in, const Tree< std::string, Graph >& graphs)
{
  std::string graphName;
  std::string vertex;
  in >> graphName >> vertex;
  auto it = graphs.find(graphName);
  if (it == graphs.end() || it->second.vertices.find(vertex) == it->second.vertices.end())
  {
    throw std::invalid_argument("Invalid command");
  }
  auto edgesIt = it->second.edges.find(vertex);
  if (edgesIt == it->second.edges.end())
  {
    out << "\n";
    return;
  }
  Array< std::string > targets;
  Tree< std::string, Array< size_t > > weightsMap;
  for (auto it = edgesIt->value.begin(); it != edgesIt->value.end(); ++it)
  {
    targets.push_back(it->key);
    weightsMap.insert({it->key, it->value});
  }
  for (size_t i = 0; i < targets.size(); ++i)
  {
    for (size_t j = i + 1; j < targets.size(); ++j)
    {
      if (targets[j] < targets[i])
      {
        std::swap(targets[i], targets[j]);
      }
    }
  }
  if (targets.empty())
  {
    out << "\n";
    return;
  }
  for (size_t i = 0; i < targets.size(); ++i)
  {
    auto weightsIt = weightsMap.find(targets[i]);
    out << targets[i];
    Array< size_t > weights = weightsIt->second;
    for (size_t j = 0; j < weights.size(); ++j)
    {
      for (size_t k = j + 1; k < weights.size(); ++k)
      {
        if (weights[k] < weights[j])
        {
          std::swap(weights[j], weights[k]);
        }
      }
    }
    for (size_t j = 0; j < weights.size(); ++j)
    {
      out << " " << weights[j];
    }
    out << "\n";
  }
}

void averenkov::printInbound(std::ostream& out, std::istream& in, const Tree< std::string, Graph >& graphs)
{
  std::string graphName, vertex;
  in >> graphName >> vertex;
  auto graphIt = graphs.find(graphName);
  if (graphIt == graphs.end())
  {
    throw std::invalid_argument("Graph not found");
  }
  if (graphIt->second.vertices.find(vertex) == graphIt->second.vertices.end())
  {
    throw std::invalid_argument("Invalid command");
  }
  Array< std::string > sources;
  Tree< std::string, Array< size_t > > weightsMap;
  for (auto fromIt = graphIt->second.edges.begin(); fromIt != graphIt->second.edges.end(); ++fromIt)
  {
    for (auto toIt = fromIt->value.begin(); toIt != fromIt->value.end(); ++toIt)
    {
      if (toIt->key == vertex)
      {
        sources.push_back(fromIt->key);
        weightsMap.insert({fromIt->key, toIt->value});
        break;
      }
    }
  }
  for (size_t i = 0; i < sources.size(); ++i)
  {
    for (size_t j = i + 1; j < sources.size(); ++j)
    {
      if (sources[j] < sources[i])
      {
        std::swap(sources[i], sources[j]);
      }
    }
  }
  for (size_t i = 0; i < sources.size(); ++i)
  {
    auto weightsIt = weightsMap.find(sources[i]);
    Array< size_t > weights = weightsIt->second;
    for (size_t j = 0; j < weights.size(); ++j)
    {
      for (size_t k = j + 1; k < weights.size(); ++k)
      {
        if (weights[k] < weights[j])
        {
          std::swap(weights[j], weights[k]);
        }
      }
    }
    out << sources[i];
    for (size_t j = 0; j < weights.size(); ++j)
    {
      out << " " << weights[j];
    }
    out << "\n";
  }
}

void averenkov::bindEdge(std::istream& in, Tree< std::string, Graph >& graphs)
{
  std::string graphName;
  std::string from;
  std::string to;
  size_t weight;
  in >> graphName >> from >> to >> weight;
  auto it = graphs.find(graphName);
  if (it == graphs.end())
  {

    throw std::invalid_argument("Invalid command");
  }
  it->second.addEdge(from, to, weight);
}

void averenkov::cutEdge(std::istream& in, Tree< std::string, Graph >& graphs)
{
  std::string graphName, from, to;
  size_t weight;
  in >> graphName >> from >> to >> weight;
  auto graphIt = graphs.find(graphName);
  if (graphIt == graphs.end())
  {
    throw std::invalid_argument("Graph not found");
  }
  auto fromIt = graphIt->second.edges.find(from);
  if (fromIt == graphIt->second.edges.end())
  {
    throw std::invalid_argument("Edge not found");
  }
  auto toIt = fromIt->value.find(to);
  if (toIt == fromIt->value.end())
  {
    throw std::invalid_argument("Edge not found");
  }
  bool found = false;
  for (size_t i = 0; i < toIt->value.size(); ++i)
  {
    if (toIt->value[i] == weight)
    {
      for (size_t j = i; j < toIt->value.size() - 1; ++j)
      {
        toIt->value[j] = toIt->value[j + 1];
      }
      toIt->value.pop_back();
      found = true;
      break;
    }
  }
  if (!found)
  {
    throw std::invalid_argument("Weight not found");
  }
  if (toIt->value.empty())
  {
    fromIt->value.erase(to);
  }
}

void averenkov::createGraph(std::istream& in, Tree< std::string, Graph >& graphs)
{
  std::string graphName;
  in >> graphName;
  if (graphs.find(graphName) != graphs.end())
  {
    throw std::invalid_argument("Invalid command");
  }
  size_t vertexCount = 0;
  in >> vertexCount;
  Graph newGraph;
  for (size_t i = 0; i < vertexCount; ++i)
  {
    std::string vertex;
    if (!(in >> vertex))
    {
      throw std::invalid_argument("Invalid command");
    }
    newGraph.addVertex(vertex);
  }
  graphs.insert({graphName, newGraph});
}

void averenkov::mergeGraphs(std::istream& in, Tree< std::string, Graph >& graphs)
{
  std::string newGraph, graph1, graph2;
  in >> newGraph >> graph1 >> graph2;
  if (graphs.find(newGraph) != graphs.end())
  {
    throw std::invalid_argument("Graph already exists");
  }
  auto it1 = graphs.find(graph1);
  auto it2 = graphs.find(graph2);
  if (it1 == graphs.end() || it2 == graphs.end())
  {
    throw std::invalid_argument("Graph not found");
  }
  Graph mergedGraph;
  mergedGraph.name = newGraph;
  for (auto it = it1->second.vertices.begin(); it != it1->second.vertices.end(); ++it)
  {
    mergedGraph.vertices.insert({it->key, true});
  }
  for (auto it = it2->second.vertices.begin(); it != it2->second.vertices.end(); ++it)
  {
    mergedGraph.vertices.insert({it->key, true});
  }
  for (auto fromIt = it1->second.edges.begin(); fromIt != it1->second.edges.end(); ++fromIt)
  {
    for (auto toIt = fromIt->value.begin(); toIt != fromIt->value.end(); ++toIt)
    {
      auto mergedToIt = mergedGraph.edges.find(fromIt->key);
      if (mergedToIt == mergedGraph.edges.end())
      {
        HashTable< std::string, Array< size_t > > newMap;
        newMap.insert({toIt->key, toIt->value});
        mergedGraph.edges.insert({fromIt->key, newMap});
      }
      else
      {
        mergedToIt->value.insert({toIt->key, toIt->value});
      }
    }
  }
  for (auto fromIt = it2->second.edges.begin(); fromIt != it2->second.edges.end(); ++fromIt)
  {
    for (auto toIt = fromIt->value.begin(); toIt != fromIt->value.end(); ++toIt)
    {
      auto mergedToIt = mergedGraph.edges.find(fromIt->key);
      if (mergedToIt == mergedGraph.edges.end())
      {
        HashTable< std::string, Array< size_t > > newMap;
        newMap.insert({toIt->key, toIt->value});
        mergedGraph.edges.insert({fromIt->key, newMap});
      }
      else
      {
        auto existingWeightsIt = mergedToIt->value.find(toIt->key);
        if (existingWeightsIt == mergedToIt->value.end())
        {
          mergedToIt->value.insert({toIt->key, toIt->value});
        }
        else
        {
          for (size_t i = 0; i < toIt->value.size(); ++i)
          {
            existingWeightsIt->value.push_back(toIt->value[i]);
          }
        }
      }
    }
  }
  graphs.insert({newGraph, mergedGraph});
}

void averenkov::extractGraph(std::istream& in, Tree< std::string, Graph >& graphs)
{
  std::string newGraph, oldGraph;
  size_t count;
  in >> newGraph >> oldGraph >> count;
  if (graphs.find(newGraph) != graphs.end())
  {
    throw std::invalid_argument("Graph already exists");
  }
  auto oldGraphIt = graphs.find(oldGraph);
  if (oldGraphIt == graphs.end())
  {
    throw std::invalid_argument("Graph not found");
  }
  Array< std::string > vertices;
  readVertices(in, count, vertices);
  for (size_t i = 0; i < vertices.size(); ++i)
  {
    if (oldGraphIt->second.vertices.find(vertices[i]) == oldGraphIt->second.vertices.end())
    {
      throw std::invalid_argument("Vertex not found in source graph");
    }
  }
  Graph extractedGraph;
  extractedGraph.name = newGraph;
  for (size_t i = 0; i < vertices.size(); ++i)
  {
    extractedGraph.vertices.insert({vertices[i], true});
  }
  for (size_t i = 0; i < vertices.size(); ++i)
  {
    auto fromIt = oldGraphIt->second.edges.find(vertices[i]);
    if (fromIt != oldGraphIt->second.edges.end())
    {
      for (auto toIt = fromIt->value.begin(); toIt != fromIt->value.end(); ++toIt)
      {
        bool targetFound = false;
        for (size_t j = 0; j < vertices.size(); ++j)
        {
          if (vertices[j] == toIt->key)
          {
            targetFound = true;
            break;
          }
        }
        if (targetFound)
        {
          auto extractedToIt = extractedGraph.edges.find(vertices[i]);
          if (extractedToIt == extractedGraph.edges.end())
          {
            HashTable< std::string, Array< size_t > > newMap;
            newMap.insert({toIt->key, toIt->value});
            extractedGraph.edges.insert({vertices[i], newMap});
          }
          else
          {
            extractedToIt->value.insert({toIt->key, toIt->value});
          }
        }
      }
    }
  }
  graphs.insert({newGraph, extractedGraph});
}

void averenkov::commandsInit(Tree< std::string, std::function< void() > >& cmds, Tree< std::string, Graph >& graphs)
{
  cmds["graphs"] = std::bind(printGraphs, std::ref(std::cout), std::cref(graphs));
  cmds["vertexes"] = std::bind(printVertices, std::ref(std::cout), std::ref(std::cin), std::cref(graphs));
  cmds["outbound"] = std::bind(printOutbound, std::ref(std::cout), std::ref(std::cin), std::cref(graphs));
  cmds["inbound"] = std::bind(printInbound, std::ref(std::cout), std::ref(std::cin), std::cref(graphs));
  cmds["bind"] = std::bind(bindEdge, std::ref(std::cin), std::ref(graphs));
  cmds["cut"] = std::bind(cutEdge, std::ref(std::cin), std::ref(graphs));
  cmds["create"] = std::bind(createGraph, std::ref(std::cin), std::ref(graphs));
  cmds["merge"] = std::bind(mergeGraphs, std::ref(std::cin), std::ref(graphs));
  cmds["extract"] = std::bind(extractGraph, std::ref(std::cin), std::ref(graphs));
}
