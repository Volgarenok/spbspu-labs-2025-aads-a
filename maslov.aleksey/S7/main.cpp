#include <functional>
#include <iostream>
#include <fstream>
#include <limits>
#include "commands.hpp"

namespace
{
  void readGraphs(std::istream & in, std::map< std::string, maslov::Graph > & graphs)
  {
    std::string graphName;
    size_t edgeCount = 0;
    while (in >> graphName >> edgeCount)
    {
      maslov::Graph graph;
      for (size_t i = 0; i < edgeCount; ++i)
      {
        std::string vertexName1, vertexName2;
        int weight = 0;
        in >> vertexName1 >> vertexName2 >> weight;
        graph.addEdge(vertexName1, vertexName2, weight);
      }
      graphs[graphName] = graph;
    }
  }
}

int main(int argc, char * argv[])
{
  if (argc != 2)
  {
    std::cerr << "ERROR: wrong arguments\n";
    return 1;
  }
  std::ifstream fileInput(argv[1]);
  if (!fileInput.is_open())
  {
    std::cerr << "ERROR: cannot open the file\n";
    return 1;
  }

  std::map< std::string, maslov::Graph > graphs;
  std::map< std::string, std::function< void() > > cmds;
  cmds["graphs"] = std::bind(maslov::getGraphs, std::ref(std::cout), std::cref(graphs));
  cmds["vertexes"] = std::bind(maslov::getVertexes, std::ref(std::cin), std::ref(std::cout), std::cref(graphs));
  cmds["outbound"]  = std::bind(maslov::getOutbound, std::ref(std::cin), std::ref(std::cout), std::cref(graphs));
  cmds["inbound"] = std::bind(maslov::getInbound, std::ref(std::cin), std::ref(std::cout), std::cref(graphs));
  cmds["bind"] = std::bind(maslov::createEdge, std::ref(std::cin), std::ref(graphs));
  cmds["cut"] = std::bind(maslov::deleteEdge, std::ref(std::cin), std::ref(graphs));
  cmds["create"] = std::bind(maslov::createGraph, std::ref(std::cin), std::ref(graphs));
  cmds["merge"] = std::bind(maslov::mergeGraph, std::ref(std::cin), std::ref(graphs));
  cmds["extract"] = std::bind(maslov::extractGraph, std::ref(std::cin), std::ref(graphs));

  readGraphs(fileInput, graphs);
  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      cmds.at(command)();
    }
    catch (const std::exception &)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}
