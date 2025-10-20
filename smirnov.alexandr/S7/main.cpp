#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <limits>
#include <cstddef>
#include "graphCollection.hpp"
#include "commands.hpp"

namespace
{
  void loadGraphs(std::istream & input, smirnov::GraphCollection & graphs)
  {
    std::string graphName;
    size_t edgeCount = 0;
    while (input >> graphName >> edgeCount)
    {
      if (graphName.empty())
      {
        input.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
        continue;
      }
      smirnov::Graph graph(graphName);
      bool edgeError = false;
      for (size_t i = 0; i < edgeCount; ++i)
      {
        std::string from, to;
        size_t weight = 0;
        if (!(input >> from >> to >> weight))
        {
          edgeError = true;
          break;
        }
        graph.addEdge(from, to, weight);
      }
      if (edgeError)
      {
        throw std::runtime_error("Input file format error\n");
      }
      if (!graphs.addGraph(graphName))
      {
        throw std::runtime_error("Duplicate graph name in input file\n");
      }
      smirnov::Graph * addedGraph = graphs.getGraph(graphName);
      if (addedGraph)
      {
        *addedGraph = graph;
      }
    }
  }
}

int main(int argc, char * argv[])
{
  using namespace smirnov;
  if (argc < 2)
  {
    std::cerr << "Input filename required\n";
    return 1;
  }
  std::ifstream inputFile(argv[1]);
  if (!inputFile.is_open())
  {
    std::cerr << "Cannot open file\n";
    return 1;
  }
  GraphCollection graphs_collection;;
  try
  {
    loadGraphs(inputFile, graphs_collection);
  }
  catch (const std::exception & e)
  {
    std::cerr << "Error loading graphs\n";
    return 1;
  }
  using namespace std::placeholders;
  HashTable< std::string, std::function< void() > > commands;
  commands["graphs"] = std::bind(graphs, std::cref(graphs_collection), std::ref(std::cout));
  commands["vertexes"] = std::bind(vertexes, std::cref(graphs_collection), std::ref(std::cin), std::ref(std::cout));
  commands["outbound"] = std::bind(outbound, std::cref(graphs_collection), std::ref(std::cin), std::ref(std::cout));
  commands["inbound"] = std::bind(inbound, std::cref(graphs_collection), std::ref(std::cin), std::ref(std::cout));
  commands["bind"] = std::bind(bind, std::ref(graphs_collection), std::ref(std::cin));
  commands["cut"] = std::bind(cut, std::ref(graphs_collection), std::ref(std::cin));
  commands["create"] = std::bind(create, std::ref(graphs_collection), std::ref(std::cin));
  commands["merge"] = std::bind(merge, std::ref(graphs_collection), std::ref(std::cin));
  commands["extract"] = std::bind(extract, std::ref(graphs_collection), std::ref(std::cin));
  std::string command;
  while (std::cin >> command)
  {
    try
    {
      commands.at(command)();
    }
    catch (const std::exception &)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
