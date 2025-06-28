#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <tree/TwoThreeTree.hpp>
#include "commands.hpp"

namespace {
  void loadDatasetsFromFile(const std::string& filename, gavrilova::Dataset& datasets)
  {
    std::ifstream file(filename);
    if (!file) {
      throw std::runtime_error("Failed to open file");
    }

    std::string datasetName;
    while (file >> datasetName) {
      gavrilova::KeyMap dataset;
      size_t key;
      while (file >> key) {
        std::string value;
        if (!(file >> value)) {
          throw std::runtime_error("Invalid data format");
        }
        dataset.insert({key, value});
      }
      datasets.insert({datasetName, std::move(dataset)});
      file.clear();
    }
  }

  void handlePrint(gavrilova::Dataset& datasets,
      std::istream& in, std::ostream& out)
  {
    std::string datasetName;
    in >> datasetName;
    gavrilova::printDataset(out, datasetName, datasets);
  }

  void handleComplement(gavrilova::Dataset& datasets,
      std::istream& in, std::ostream&)
  {
    std::string newName, firstName, secondName;
    in >> newName >> firstName >> secondName;
    gavrilova::complementDataset(newName, firstName, secondName, datasets);
  }

  void handleIntersect(gavrilova::Dataset& datasets,
      std::istream& in, std::ostream&)
  {
    std::string newName, firstName, secondName;
    in >> newName >> firstName >> secondName;
    gavrilova::intersectDatasets(newName, firstName, secondName, datasets);
  }

  void handleUnion(gavrilova::Dataset& datasets,
      std::istream& in, std::ostream&)
  {
    std::string newName, firstName, secondName;
    in >> newName >> firstName >> secondName;
    gavrilova::unionDatasets(newName, firstName, secondName, datasets);
  }
}

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cerr << "ERROR: Incorrect file.\n";
    return 1;
  }

  gavrilova::Dataset datasets;
  try {
    loadDatasetsFromFile(argv[1], datasets);
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  using namespace std::placeholders;
  gavrilova::TwoThreeTree< std::string, std::function< void(gavrilova::Dataset&) > > commands;

  commands.insert({"print",
      std::bind(handlePrint, _1, std::ref(std::cin), std::ref(std::cout))});
  commands.insert({"complement",
      std::bind(handleComplement, _1, std::ref(std::cin), std::ref(std::cout))});
  commands.insert({"intersect",
      std::bind(handleIntersect, _1, std::ref(std::cin), std::ref(std::cout))});
  commands.insert({"union",
      std::bind(handleUnion, _1, std::ref(std::cin), std::ref(std::cout))});

  std::string command;
  while (std::cin >> command) {
    try {
      auto it = commands.find(command);
      if (it == commands.end()) {
        throw std::invalid_argument("Unknown command");
      }
      it->second(datasets);
    } catch (const std::invalid_argument&) {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    } catch (const std::exception& e) {
      std::cerr << "Error: " << e.what() << "\n";
    }
  }
}
