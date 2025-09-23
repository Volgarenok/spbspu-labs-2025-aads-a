#include "commands.hpp"
#include <iostream>

namespace
{
  void printBounds(std::ostream & out, const std::vector< std::pair< std::string, std::vector< int > > > & bounds)
  {
    if (bounds.empty())
    {
      out << '\n';
    }
    for (auto it = bounds.cbegin(); it != bounds.cend(); it++)
    {
      out << it->first;
      for (auto itWeight = it->second.cbegin(); itWeight != it->second.cend(); itWeight++)
      {
        out << ' ' << *itWeight;
      }
      out << '\n';
    }
  }
}

void maslov::getGraphs(std::ostream & out, const map & graphs)
  {
    if (graphs.empty())
    {
      out << '\n';
    }
    for (auto it = graphs.cbegin(); it != graphs.cend(); it++)
    {
      out << it->first << '\n';
    }
  }

  void maslov::getVertexes(std::istream & in, std::ostream & out, const map & graphs)
  {
    std::string graphName;
    in >> graphName;
    const Graph & graph = graphs.at(graphName);
    std::vector< std::string > vertexes = graph.getVertexes();
    if (vertexes.empty())
    {
      out << '\n';
    }
    for (auto it = vertexes.begin(); it != vertexes.end(); it++)
    {
      out << *it << '\n';
    }
  }

  void maslov::getOutbound(std::istream & in, std::ostream & out, const map & graphs)
  {
    std::string graphName, vertexName;
    in >> graphName >> vertexName;
    const Graph & graph = graphs.at(graphName);
    if (!graph.hasVertex(vertexName))
    {
      throw std::logic_error("ERROR: there is no such vertex");
    }
    auto outbounds = graph.getOutbound(vertexName);
    printBounds(out, outbounds);
  }

  void maslov::getInbound(std::istream & in, std::ostream & out, const map & graphs)
  {
    std::string graphName, vertexName;
    in >> graphName >> vertexName;
    const Graph & graph = graphs.at(graphName);
    if (!graph.hasVertex(vertexName))
    {
      throw std::logic_error("ERROR: there is no such vertex");
    }
    auto inbounds = graph.getInbound(vertexName);
    printBounds(out, inbounds);
  }

  void maslov::createEdge(std::istream & in, map & graphs)
  {
    std::string graphName, v1, v2;
    int weight = 0;
    in >> graphName >> v1 >> v2 >> weight;
    Graph & graph = graphs.at(graphName);
    graph.addEdge(v1, v2, weight);
  }

  void maslov::deleteEdge(std::istream & in, map & graphs)
  {
    std::string graphName, v1, v2;
    int weight = 0;
    in >> graphName >> v1 >> v2 >> weight;
    Graph & graph = graphs.at(graphName);
    graph.deleteEdge(v1, v2, weight);
  }

  void maslov::createGraph(std::istream & in, map & graphs)
  {
    std::string graphName;
    in >> graphName;
    if (graphs.find(graphName) != graphs.end())
    {
      throw std::logic_error("ERROR: such a graph already exists");
    }
    Graph newGraph;
    size_t vertexesCount = 0;
    in >> vertexesCount;
    std::string vertex;
    for (size_t i = 0; i < vertexesCount; i++)
    {
      in >> vertex;
      newGraph.addVertex(vertex);
    }
    graphs[graphName] = newGraph;
  }

  void maslov::mergeGraph(std::istream & in, map & graphs)
  {
    std::string newGraphName, graphName1, graphName2;
    in >> newGraphName >> graphName1 >> graphName2;
    if (graphs.find(newGraphName) != graphs.end())
    {
      throw std::logic_error("ERROR: such a graph already exists");
    }
    const Graph & graph1 = graphs.at(graphName1);
    const Graph & graph2 = graphs.at(graphName2);
    Graph mergedGraph = graph1;
    mergedGraph.merge(graph2);
    graphs[newGraphName] = mergedGraph;
  }

  void maslov::extractGraph(std::istream & in, map & graphs)
  {
    std::string newGraphName, graphName;
    in >> newGraphName >> graphName;
    if (graphs.find(newGraphName) != graphs.end())
    {
      throw std::logic_error("ERROR: such a graph already exists");
    }
    const Graph & graph = graphs.at(graphName);
    size_t countVertexes = 0;
    in >> countVertexes;
    std::set< std::string > vertexes;
    std::string vertex;
    for (size_t i = 0; i < countVertexes; ++i)
    {
      in >> vertex;
      graph.hasVertex(vertex);
      vertexes.insert(vertex);
    }
    Graph extractedGraph;
    extractedGraph.extract(graph, vertexes);
    graphs[newGraphName] = extractedGraph;
  }
