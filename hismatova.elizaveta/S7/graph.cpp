#include <sstream>
#include "graph.hpp"

std::vector< std::string > hismatova::GraphProcessor::split(const std::string& str)
{
  std::vector< std::string > tokens;
  std::istringstream iss(str);
  std::string token;

  while (iss >> token)
  {
    tokens.push_back(token);
  }

  return tokens;
}

bool hismatova::GraphProcessor::graphExists(const std::string& graphName)
{
  return graphs.contains(graphName);
}

bool hismatova::GraphProcessor::vertexExists(const std::string& graphName, const std::string& vertex)
{
  if (!graphExists(graphName))
  {
    return false;
  }
  return vertices[graphName].count(vertex) > 0;
}

void hismatova::GraphProcessor::addVertex(const std::string& graphName, const std::string& vertex)
{
  vertices[graphName].insert(vertex);
  if (!graphs[graphName].contains(vertex))
  {
    graphs[graphName][vertex] = HashTable< std::string, std::vector< int > >();
  }
  if (!inverse_graphs[graphName].contains(vertex))
  {
    inverse_graphs[graphName][vertex] = HashTable< std::string, std::vector< int > >();
  }
}

void hismatova::GraphProcessor::addEdge(const std::string& graphName, const std::string& from, const std::string& to, int weight)
{
  addVertex(graphName, from);
  addVertex(graphName, to);

  graphs[graphName][from][to].push_back(weight);
  inverse_graphs[graphName][to][from].push_back(weight);
}

void hismatova::GraphProcessor::removeEdge(const std::string& graphName, const std::string& from, const std::string& to, int weight)
{
  if (!graphExists(graphName))
  {
    return;
  }
  if (!graphs[graphName].contains(from) || !graphs[graphName][from].contains(to))
  {
    return;
  }

  auto& weights = graphs[graphName][from][to];
  for (auto it = weights.begin(); it != weights.end(); )
  {
    if (*it == weight)
    {
      it = weights.erase(it);
      break;
    }
    else
    {
      ++it;
    }
  }
  if (weights.empty())
  {
    graphs[graphName][from].erase(to);
  }

  auto& inv_weights = inverse_graphs[graphName][to][from];
  for (auto it = inv_weights.begin(); it != inv_weights.end(); )
  {
    if (*it == weight)
    {
      it = inv_weights.erase(it);
      break;
    }
    else
    {
      ++it;
    }
  }
  if (inv_weights.empty())
  {
    inverse_graphs[graphName][to].erase(from);
  }
}

void hismatova::GraphProcessor::loadGraphsFromFile(const std::string& filename)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    return;
  }

  std::string line;
  std::string currentGraph;
  int edgeCount = 0;
  int edgesRead = 0;
  bool inGraphSection = false;

  while (std::getline(file, line))
  {
    if (line.empty())
    {
      continue;
    }

    std::vector< std::string > tokens = split(line);
    if (tokens.empty())
    {
      continue;
    }

    if (tokens.size() >= 2 && !inGraphSection)
    {
      currentGraph = tokens[0];
      try
      {
        edgeCount = std::stoi(tokens[1]);
        edgesRead = 0;
        inGraphSection = true;

        graphs[currentGraph] = HashTable< std::string, HashTable< std::string, std::vector< int > > >();
        inverse_graphs[currentGraph] = HashTable< std::string, HashTable< std::string, std::vector< int > > >();
        vertices[currentGraph] = std::set< std::string >();
      }
      catch (...)
      {
        continue;
      }
    }
    else if (tokens.size() == 3 && inGraphSection && edgesRead < edgeCount)
    {
      std::string from = tokens[0];
      std::string to = tokens[1];
      int weight;
      try
      {
        weight = std::stoi(tokens[2]);
      }
      catch (...)
      {
        continue;
      }

      addEdge(currentGraph, from, to, weight);
      edgesRead++;

      if (edgesRead >= edgeCount)
      {
        inGraphSection = false;
      }
    }
    else if (inGraphSection && tokens.size() >= 2)
    {
      inGraphSection = false;
    }
  }

  file.close();
}

void hismatova::GraphProcessor::executeGraphs(const std::vector< std::string >&)
{
  if (graphs.empty())
  {
    return;
  }

  std::set<std::string> graphNames;
  for (auto it = graphs.begin(); it != graphs.end(); ++it)
  {
    graphNames.insert(it->first);
  }
  for (const auto& name: graphNames)
  {
    std::cout << name << std::endl;
  }
}

void hismatova::GraphProcessor::executeVertexes(const std::vector< std::string >& tokens)
{
  std::string graphName = tokens[1];
  if (!graphExists(graphName))
  {
    std::cout << "<INVALID COMMAND>" << std::endl;
    return;
  }

  if (vertices[graphName].empty())
  {
    return;
  }

  for (const auto& vertex: vertices[graphName])
  {
    std::cout << vertex << std::endl;
  }
}

void hismatova::GraphProcessor::executeOutbound(const std::vector< std::string >& tokens)
{
  std::string graphName = tokens[1];
  std::string vertex = tokens[2];

  if (!graphExists(graphName) || !vertexExists(graphName, vertex))
  {
    std::cout << "<INVALID COMMAND>" << std::endl;
    return;
  }

  bool found = false;
  if (graphs[graphName].contains(vertex))
  {
    std::vector< std::pair< std::string, int > > allEdges;
    auto& outMap = graphs[graphName][vertex];
    for (auto it = outMap.begin(); it != outMap.end(); ++it)
    {
      const auto& target = it->first;
      const auto& weights = it->second;
      for (int w: weights)
      {
        allEdges.push_back({target, w});
        found = true;
      }
    }

    std::sort(allEdges.begin(), allEdges.end(),
      [](const std::pair< std::string, int >& a, const std::pair< std::string, int >& b)
      {
        if (a.first != b.first)
        {
          return a.first < b.first;
        }
        return a.second < b.second;
      });

    for (const auto& edge: allEdges)
    {
      std::cout << edge.first << " " << edge.second << std::endl;
    }
  }

  if (!found)
  {
    std::cout << "<INVALID COMMAND>" << std::endl;
  }
}

void hismatova::GraphProcessor::executeInbound(const std::vector< std::string >& tokens)
{
  std::string graphName = tokens[1];
  std::string vertex = tokens[2];

  if (!graphExists(graphName) || !vertexExists(graphName, vertex))
  {
    std::cout << "<INVALID COMMAND>" << std::endl;
    return;
  }

  bool found = false;
  if (inverse_graphs[graphName].contains(vertex))
  {
    found = false;
    std::vector< std::pair< std::string, int > > allEdges;
    auto& inMap = inverse_graphs[graphName][vertex];
    for (auto it = inMap.begin(); it != inMap.end(); ++it)
    {
      const auto& source = it->first;
      const auto& weights = it->second;
      for (int w: weights)
      {
        allEdges.push_back({source, w});
        found = true;
      }
    }

    std::sort(allEdges.begin(), allEdges.end(),
      [](const std::pair< std::string, int >& a, const std::pair< std::string, int >& b)
      {
        if (a.first != b.first)
        {
          return a.first < b.first;
        }
        return a.second < b.second;
      });

    for (const auto& edge : allEdges)
    {
      std::cout << edge.first << " " << edge.second << std::endl;
    }
  }
  if (!found)
  {
    std::cout << "<INVALID COMMAND>" << std::endl;
  }
}

void hismatova::GraphProcessor::executeBind(const std::vector< std::string >& tokens)
{
  std::string graphName = tokens[1];
  std::string from = tokens[2];
  std::string to = tokens[3];
  int weight;
  try
  {
    weight = std::stoi(tokens[4]);
  }
  catch (...)
  {
    std::cout << "<INVALID COMMAND>" << std::endl;
    return;
  }

  if (!graphExists(graphName))
  {
    std::cout << "<INVALID COMMAND>" << std::endl;
    return;
  }

  addEdge(graphName, from, to, weight);
}

void hismatova::GraphProcessor::executeCut(const std::vector< std::string >& tokens)
{
  std::string graphName = tokens[1];
  std::string from = tokens[2];
  std::string to = tokens[3];
  int weight;
  try
  {
    weight = std::stoi(tokens[4]);
  }
  catch (...)
  {
    std::cout << "<INVALID COMMAND>" << std::endl;
    return;
  }

  if (!graphExists(graphName))
  {
    std::cout << "<INVALID COMMAND>" << std::endl;
    return;
  }

  if (!graphs[graphName].contains(from) ||
    !graphs[graphName][from].contains(to) ||
    std::find(graphs[graphName][from][to].begin(),
          graphs[graphName][from][to].end(), weight) == graphs[graphName][from][to].end())
  {
    std::cout << "<INVALID COMMAND>" << std::endl;
    return;
  }

  removeEdge(graphName, from, to, weight);
}

void hismatova::GraphProcessor::executeCreate(const std::vector< std::string >& tokens)
{
  std::string graphName = tokens[1];
  if (graphExists(graphName))
  {
    std::cout << "<INVALID COMMAND>" << std::endl;
    return;
  }

  size_t count;
  try
  {
    count = std::stoi(tokens[2]);
  }
  catch (...)
  {
    std::cout << "<INVALID COMMAND>" << std::endl;
    return;
  }

  if (tokens.size() != 3 + count)
  {
    std::cout << "<INVALID COMMAND>" << std::endl;
    return;
  }

  graphs[graphName] = HashTable< std::string, HashTable< std::string, std::vector< int > > >();
  inverse_graphs[graphName] = HashTable< std::string, HashTable< std::string, std::vector< int > > >();
  vertices[graphName] = std::set<std::string>();

  for (size_t i = 0; i < count; i++)
  {
    addVertex(graphName, tokens[3 + i]);
  }
}

void hismatova::GraphProcessor::executeMerge(const std::vector< std::string >& tokens)
{
  std::string newGraph = tokens[1];
  std::string graph1 = tokens[2];
  std::string graph2 = tokens[3];

  if (graphExists(newGraph) || !graphExists(graph1) || !graphExists(graph2))
  {
    std::cout << "<INVALID COMMAND>" << std::endl;
    return;
  }

  graphs[newGraph] = HashTable< std::string, HashTable< std::string, std::vector< int > > >();
  inverse_graphs[newGraph] = HashTable< std::string, HashTable< std::string, std::vector< int > > >();
  vertices[newGraph] = std::set<std::string>();

  std::set< std::tuple< std::string, std::string, int > > addedEdges;

  for (const auto& fromVertex: vertices[graph1])
  {
    addVertex(newGraph, fromVertex);
    if (graphs[graph1].contains(fromVertex))
    {
      auto& outMap = graphs[graph1][fromVertex];
      for (auto it = outMap.begin(); it != outMap.end(); ++it)
      {
        const auto& toVertex = it->first;
        const auto& weights = it->second;
        for (int w: weights)
        {
          auto edge = std::make_tuple(fromVertex, toVertex, w);
          if (addedEdges.find(edge) == addedEdges.end())
          {
            addEdge(newGraph, fromVertex, toVertex, w);
            addedEdges.insert(edge);
          }
        }
      }
    }
  }

  for (const auto& fromVertex: vertices[graph2])
  {
    addVertex(newGraph, fromVertex);
    if (graphs[graph2].contains(fromVertex))
    {
      auto& outMap = graphs[graph2][fromVertex];
      for (auto it = outMap.begin(); it != outMap.end(); ++it)
      {
        const auto& toVertex = it->first;
        const auto& weights = it->second;
        for (int w: weights)
        {
          auto edge = std::make_tuple(fromVertex, toVertex, w);
          if (addedEdges.find(edge) == addedEdges.end())
          {
            addEdge(newGraph, fromVertex, toVertex, w);
            addedEdges.insert(edge);
          }
        }
      }
    }
  }
}

void hismatova::GraphProcessor::executeExtract(const std::vector< std::string >& tokens)
{
  std::string newGraph = tokens[1];
  std::string oldGraph = tokens[2];
  size_t count;
  try
  {
    count = std::stoi(tokens[3]);
  }
  catch (...)
  {
    std::cout << "<INVALID COMMAND>" << std::endl;
    return;
  }

  if (graphExists(newGraph) || !graphExists(oldGraph) || tokens.size() != 4 + count)
  {
    std::cout << "<INVALID COMMAND>" << std::endl;
    return;
  }

  graphs[newGraph] = HashTable< std::string, HashTable< std::string, std::vector< int > > >();
  inverse_graphs[newGraph] = HashTable<std::string, HashTable< std::string, std::vector< int > > >();
  vertices[newGraph] = std::set<std::string>();

  std::set< std::string > selectedVertices;
  for (size_t i = 0; i < count; i++)
  {
    std::string vertex = tokens[4 + i];
    if (!vertexExists(oldGraph, vertex))
    {
      std::cout << "<INVALID COMMAND>" << std::endl;
      return;
    }
    selectedVertices.insert(vertex);
    addVertex(newGraph, vertex);
  }

  for (const auto& fromVertex : selectedVertices)
  {
    if (graphs[oldGraph].contains(fromVertex))
    {
      auto& outMap = graphs[oldGraph][fromVertex];
      for (auto it = outMap.begin(); it != outMap.end(); ++it)
      {
        const auto& toVertex = it->first;
        if (selectedVertices.count(toVertex))
        {
          const auto& weights = it->second;
          for (int w: weights)
          {
            addEdge(newGraph, fromVertex, toVertex, w);
          }
        }
      }
    }
  }
}

void hismatova::GraphProcessor::executeCommand(const std::string& commandLine)
{
  std::vector< std::string > tokens = split(commandLine);
  if (tokens.empty())
  {
    std::cout << "<INVALID COMMAND>" << std::endl;
    return;
  }

  std::string command = tokens[0];

  if (command == "graphs")
  {
    executeGraphs(tokens);
  }
  else if (command == "vertexes" && tokens.size() == 2)
  {
    executeVertexes(tokens);
  }
  else if (command == "outbound" && tokens.size() == 3)
  {
    executeOutbound(tokens);
  }
  else if (command == "inbound" && tokens.size() == 3)
  {
    executeInbound(tokens);
  }
  else if (command == "bind" && tokens.size() == 5)
  {
    executeBind(tokens);
  }
  else if (command == "cut" && tokens.size() == 5)
  {
    executeCut(tokens);
  }
  else if (command == "create" && tokens.size() >= 3)
  {
    executeCreate(tokens);
  }
  else if (command == "merge" && tokens.size() == 4)
  {
    executeMerge(tokens);
  }
  else if (command == "extract" && tokens.size() >= 4)
  {
    executeExtract(tokens);
  }
  else
  {
    std::cout << "<INVALID COMMAND>" << std::endl;
  }
}
