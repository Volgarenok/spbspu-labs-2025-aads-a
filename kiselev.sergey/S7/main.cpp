#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <limits>
#include "commands.hpp"
int main(int argc, char** argv)
{
  using namespace kiselev;
  if (argc != 2)
  {
    std::cerr << "Incorrect parameter\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  Graphs graph;
  input(file, graph);
  std::map< std::string, std::function< void() > > commands;
  using namespace std::placeholders;
  commands.insert(std::make_pair("graphs", std::bind(graphs, std::ref(std::cout), std::cref(graph))));
  commands.insert(std::make_pair("vertexes", std::bind(vertexes, std::ref(std::cout), std::ref(std::cin), std::cref(graph))));
  commands.insert(std::make_pair("outbound", std::bind(outBound, std::ref(std::cout), std::ref(std::cin), std::cref(graph))));
  commands.insert(std::make_pair("inbound", std::bind(inBound, std::ref(std::cout), std::ref(std::cin), std::cref(graph))));
  commands.insert(std::make_pair("bind", std::bind(bind, std::ref(std::cin), std::ref(graph))));
  commands.insert(std::make_pair("cut", std::bind(cut, std::ref(std::cin), std::ref(graph))));
  commands.insert(std::make_pair("create", std::bind(create, std::ref(std::cin), std::ref(graph))));
  commands.insert(std::make_pair("merge", std::bind(merge, std::ref(std::cin), std::ref(graph))));
  commands.insert(std::make_pair("extract", std::bind(extract, std::ref(std::cin), std::ref(graph))));
  std::string command;
  while (std::cin >> command)
  {
    try
    {
      commands.at(command)();
    }
    catch (const std::exception&)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
