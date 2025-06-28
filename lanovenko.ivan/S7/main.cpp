#include <limits>
#include <fstream>
#include <iostream>
#include <functional>
#include "command_processing.hpp"

namespace
{
  void input_graphs(std::istream& in, std::map< std::string, lanovenko::Graph >& graphs)
  {
    std::string graphName;
    size_t edgeCount = 0;
    while (in >> graphName >> edgeCount)
    {
      lanovenko::Graph graph;
      for (size_t i = 0; i < edgeCount; ++i)
      {
        std::string vertexName1, vertexName2;
        int weight = 0;
        in >> vertexName1 >> vertexName2 >> weight;
        graph.add_edge(vertexName1, vertexName2, weight);
      }
      graphs[graphName] = graph;
    }
  }
}

int main(int argc, char** argv)
{
  using namespace lanovenko;
  if (argc != 2)
  {
    std::cerr << "<WRONG ARGUMENT COUNT>\n";
    return 1;
  }
  std::ifstream finput(argv[1]);
  std::map< std::string, Graph > graphs;
  std::map< std::string, std::function< void() > > cmds;
  cmds["graphs"] = std::bind(get_graphs, std::ref(std::cout), std::cref(graphs));
  cmds["vertexes"] = std::bind(get_vertexes, std::ref(std::cin), std::ref(std::cout), std::cref(graphs));
  cmds["outbound"] = std::bind(get_outbound, std::ref(std::cin), std::ref(std::cout), std::cref(graphs));
  cmds["inbound"] = std::bind(get_inbound, std::ref(std::cin), std::ref(std::cout), std::cref(graphs));
  cmds["bind"] = std::bind(create_edge, std::ref(std::cin), std::ref(graphs));
  cmds["cut"] = std::bind(delete_edge, std::ref(std::cin), std::ref(graphs));
  cmds["create"] = std::bind(create_graph, std::ref(std::cin), std::ref(graphs));
  cmds["merge"] = std::bind(merge_graph, std::ref(std::cin), std::ref(graphs));
  cmds["extract"] = std::bind(extract_graph, std::ref(std::cin), std::ref(graphs));

  input_graphs(finput, graphs);
  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      cmds.at(command)();
    }
    catch (const std::exception&)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}
