#include <fstream>
#include <iostream>
#include <limits>

#include <tree/tree.hpp>
#include "tree_actions.hpp"

namespace {

  zholobov::NamedTrees readTrees(std::istream& input)
  {
    zholobov::NamedTrees result;
    while (!input.eof()) {
      input.clear();
      zholobov::TreeMap map;
      std::string datasetName;
      input >> datasetName;
      while (input.good()) {
        size_t key = 0;
        std::string value;
        if (input >> key >> value) {
          map.emplace(key, value);
        }
      }
      result.emplace(datasetName, map);
    }
    return result;
  }

}

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cerr << "Missed required parameter <filename>\n";
    return 1;
  }

  std::ifstream input_file(argv[1]);
  zholobov::NamedTrees namedTrees = readTrees(input_file);

  zholobov::Tree< std::string, std::function< void(zholobov::NamedTrees&) > > commands{
      {"print", std::bind(zholobov::doPrint, std::ref(std::cout), std::ref(std::cin), std::placeholders::_1)},
      {"complement", std::bind(zholobov::doComplement, std::ref(std::cin), std::placeholders::_1)},
      {"intersect", std::bind(zholobov::doIntersect, std::ref(std::cin), std::placeholders::_1)},
      {"union", std::bind(zholobov::doUnion, std::ref(std::cin), std::placeholders::_1)}};

  std::string command;
  while (std::cin >> command) {
    try {
      commands.at(command)(namedTrees);
    } catch (const std::out_of_range&) {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
