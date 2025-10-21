#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "BinarySeachTree.hpp"
#include "handle_functions.hpp"
int main(int argc, char* argv[])
{
  using namespace hismatova;
  if (argc < 2)
  {
    std::cerr << "ERROR: no file name\n";
    return 1;
  }
  std::ifstream input(argv[1]);
  if (!input.is_open())
  {
    std::cerr << "ERROR: can not open file " << argv[1] << "\n";
    return 1;
  }
  std::unordered_map< std::string, BinarySearchTree< int, std::string > > datasets;
  std::string line;
  while (std::getline(input, line))
  {
    std::istringstream iss(line);
    std::string datasetName;
    if (!(iss >> datasetName))
    {
      continue;
    }
    BinarySearchTree< int, std::string > tree;
    int key;
    std::string value;
    while (iss >> key >> value)
    {
      tree.insert({key, value});
    }
    datasets[datasetName] = tree;
  }
  input.close();
  std::string commandLine;
  while (std::getline(std::cin, commandLine))
  {
    std::istringstream iss(commandLine);
    std::string command;
    iss >> command;
    if (command == "print")
    {
      std::string dataset;
      if (iss >> dataset)
      {
        if (datasets.find(dataset) == datasets.end())
        {
          std::cout << "<INVALID COMMAND>\n";
        }
        else
        {
          handle_print(datasets, dataset);
        }
      }
    }
    else if (command == "complement")
    {
      std::string result, a, b;
      if (iss >> result >> a >> b)
      {
        if (datasets.find(a) == datasets.end() || datasets.find(b) == datasets.end())
        {
          std::cout << "<INVALID COMMAND>\n";
        }
        else
        {
          handle_complement(datasets, result, a, b);
        }
      }
    }
    else if (command == "intersect")
    {
      std::string result, a, b;
      if (iss >> result >> a >> b)
      {
        if (datasets.find(a) == datasets.end() || datasets.find(b) == datasets.end())
        {
          std::cout << "<INVALID COMMAND>\n";
        }
        else
        {
          handle_intersect(datasets, result, a, b);
        }
      }
    }
    else if (command == "union")
    {
      std::string result, a, b;
      if (iss >> result >> a >> b)
      {
        if (datasets.find(a) == datasets.end() || datasets.find(b) == datasets.end())
        {
          std::cout << "<INVALID COMMAND>\n";
        }
        else
        {
          handle_union(datasets, result, a, b);
        }
      }
    }
    else if (!command.empty())
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }
  return 0;
}
