#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <functional>
#include <limits>
#include "commands.hpp"
#include "graph.hpp"
#include "robinHashTable.hpp"

namespace
{
  void loadGraphsFromFile(const std::string& filename, dribas::RobinHoodHashTable< std::string, dribas::Graph >& graphs)
  {
    std::ifstream file(filename);
    if (!file) {
      throw std::runtime_error("file");
    }
    std::string graphName;
    while (file >> graphName) {
      size_t edgeCount;
      if (!(file >> edgeCount)) {
        if (file.eof() || file.peek() == '\n') {
          graphs.insert(graphName, dribas::Graph());
          file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
          continue;
        }
        throw std::runtime_error("");
      }
      dribas::Graph graph;
      for (size_t i = 0; i < edgeCount; ++i) {
        std::string from, to;
        int weight;
        if (!(file >> from >> to >> weight)) {
          throw std::runtime_error("graph");
        }
        graph.addEdge(from, to, weight);
      }
      graphs.insert(graphName, graph);
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    if (file.bad()) {
      throw std::runtime_error("file: " + filename);
    }
  }

  void processCommands(dribas::RobinHoodHashTable<std::string, dribas::Graph>& graphs)
  {
    using namespace std::placeholders;
    std::map< std::string, std::function< void() > > commands;
    commands["graphs"] = std::bind(dribas::graph, std::ref(graphs), std::ref(std::cout));
    commands["vertexes"] = std::bind(dribas::vertexes, std::ref(graphs), std::ref(std::cin), std::ref(std::cout));
    commands["outbound"] = std::bind(dribas::outbound, std::ref(graphs), std::ref(std::cin), std::ref(std::cout));
    commands["inbound"] = std::bind(dribas::inbound, std::ref(graphs), std::ref(std::cin), std::ref(std::cout));
    commands["bind"] = std::bind(dribas::bind, std::ref(graphs), std::ref(std::cin));
    commands["cut"] = std::bind(dribas::cut, std::ref(graphs), std::ref(std::cin));
    commands["create"] = std::bind(dribas::create, std::ref(graphs), std::ref(std::cin));
    commands["merge"] = std::bind(dribas::merge, std::ref(graphs), std::ref(std::cin));
    commands["extract"] = std::bind(dribas::extract, std::ref(graphs), std::ref(std::cin));

    std::string command;
    while (std::cin >> command) {
      try {
        auto it = commands.find(command);
        if (it != commands.end()) {
          it->second();
        } else {
          throw std::invalid_argument("Unknown command");
        }
      } catch (const std::invalid_argument& e) {
        std::cout << "<INVALID COMMAND>\n";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    }
  }
}

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cerr << argv[0] << "\n";
    return 1;
  }
  dribas::RobinHoodHashTable< std::string, dribas::Graph > graphs;
  try {
    loadGraphsFromFile(argv[1], graphs);
    processCommands(graphs);
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }
  return 0;
}
