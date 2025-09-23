#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <string>

#include "HashTable.hpp"
#include "IteratorHashTable.hpp"
#include "ConstIteratorHashTable.hpp"
#include "WeightedGraph.hpp"
#include "commands.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cerr << "Invalid arguments.\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file) {
    std::cerr << "Error: Cannot open file.\n";
    return 1;
  }

  using namespace gavrilova;

  GraphsCollection graphs_collection;
  readGraphsFromFile(file, graphs_collection);

  HashTable< std::string, std::function< void() > > commands;

  commands["graphs"] = std::bind(graphs, std::ref(std::cout), std::cref(graphs_collection));
  commands["vertexes"] = std::bind(vertexes, std::ref(std::cout), std::ref(std::cin), std::cref(graphs_collection));
  commands["outbound"] = std::bind(outbound, std::ref(std::cout), std::ref(std::cin), std::cref(graphs_collection));
  commands["inbound"] = std::bind(inbound, std::ref(std::cout), std::ref(std::cin), std::cref(graphs_collection));
  commands["bind"] = std::bind(bind, std::ref(std::cin), std::ref(graphs_collection));
  commands["cut"] = std::bind(cut, std::ref(std::cin), std::ref(graphs_collection));
  commands["create"] = std::bind(create, std::ref(std::cin), std::ref(graphs_collection));
  commands["merge"] = std::bind(merge, std::ref(std::cin), std::ref(graphs_collection));
  commands["extract"] = std::bind(extract, std::ref(std::cin), std::ref(graphs_collection));

  std::string command_str;
  while (std::cin >> command_str) {
    try {
      commands.at(command_str)();
    } catch (const std::exception& e) {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
