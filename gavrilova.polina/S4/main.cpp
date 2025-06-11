#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <stdexcept>
#include "commands.hpp"

namespace {

  void loadDatasetsFromFile(const std::string& filename, gavrilova::Dataset& datasets)
  {
    std::ifstream file(filename);
    if (!file) {
      throw std::runtime_error("Failed to open file: " + filename);
    }

    std::string datasetName;
    while (file >> datasetName) {
      gavrilova::KeyMap dataset;
      size_t key;
      while (file >> key) {
        std::string value;
        if (!(file >> value)) {
          throw std::runtime_error("Invalid data format in file");
        }
        dataset.insert({key, value});
      }
      datasets.insert({datasetName, std::move(dataset)});
      file.clear();
    }
  }

  void processCommand(const std::string& command, gavrilova::Dataset& datasets)
  {
    if (command == "print") {
      std::string datasetName;
      std::cin >> datasetName;
      gavrilova::printDataset(std::cout, datasetName, datasets);
    } else if (command == "complement") {
      std::string newName, firstName, secondName;
      std::cin >> newName >> firstName >> secondName;
      gavrilova::complementDataset(newName, firstName, secondName, datasets);
    } else if (command == "intersect") {
      std::string newName, firstName, secondName;
      std::cin >> newName >> firstName >> secondName;
      gavrilova::intersectDatasets(newName, firstName, secondName, datasets);
    } else if (command == "union") {
      std::string newName, firstName, secondName;
      std::cin >> newName >> firstName >> secondName;
      gavrilova::unionDatasets(newName, firstName, secondName, datasets);
    } else {
      throw std::invalid_argument("Unknown command: " + command);
    }
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
  }

  std::string command;
  while (std::cin >> command) {
    try {
      processCommand(command, datasets);
    } catch (const std::invalid_argument&) {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    } catch (const std::exception& e) {
      std::cerr << "Error: " << e.what() << "\n";
    }
  }
}
