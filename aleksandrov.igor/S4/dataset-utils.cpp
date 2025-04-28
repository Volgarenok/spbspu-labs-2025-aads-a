#include "dataset-utils.hpp"
#include <sstream>
#include <iostream>

namespace aleksandrov
{
  DatasetCollection readDatasets(std::istream& in)
  {
    DatasetCollection datasets;
    std::string line;
    while (std::getline(in, line))
    {
      if (line.empty())
      {
        continue;
      }
      std::istringstream iss(line);
      std::string name;
      if (!(iss >> name))
      {
        continue;
      }
      Dataset dataset;
      int key = 0;
      std::string value;
      while (iss >> key >> value)
      {
        dataset.insert(std::make_pair(key, value));
      }
      datasets.insert(std::make_pair(name, dataset));
    }
    return datasets;
  }

  void printDataset(str name, const DatasetCollection& datasets)
  {
    auto it = datasets.find(name);
    if (it == datasets.end())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    const Dataset& dataset = it->second;
    if (dataset.empty())
    {
      std::cout << "<EMPTY>\n";
      return;
    }

    std::cout << name;
    for (auto it = dataset.begin(); it != dataset.end(); ++it)
    {
      std::cout << " " << it->first << " " << it->second;
    }
    std::cout << "\n";
  }

  void complementDataset(str newName, str name1, str name2, DatasetCollection& datasets)
  {
    auto it1 = datasets.find(name1);
    auto it2 = datasets.find(name2);

    if (it1 == datasets.end() || it2 == datasets.end())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    Dataset result;
    const Dataset& set1 = it1->second;
    const Dataset& set2 = it2->second;

    for (auto it = set1.begin(); it != set1.end(); ++it)
    {
      if (set2.find(it->first) == set2.end())
      {
        result.insert(std::make_pair(it->first, it->second));
      }
    }
    datasets[newName] = result;
  }

  void intersectDataset(str newName, str name1, str name2, DatasetCollection& datasets)
  {
    auto it1 = datasets.find(name1);
    auto it2 = datasets.find(name2);

    if (it1 == datasets.end() || it2 == datasets.end())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    Dataset result;
    const Dataset& set1 = it1->second;
    const Dataset& set2 = it2->second;

    for (auto it = set1.begin(); it != set1.end(); ++it)
    {
      if (set2.find(it->first) != set2.end())
      {
        result.insert(std::make_pair(it->first, it->second));
      }
    }
    datasets[newName] = result;
  }

  void unionDataset(str newName, str name1, str name2, DatasetCollection& datasets)
  {
    auto it1 = datasets.find(name1);
    auto it2 = datasets.find(name2);

    if (it1 == datasets.end() || it2 == datasets.end())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    Dataset result = it1->second;
    const Dataset& set2 = it2->second;
    for (auto it = set2.begin(); it != set2.end(); ++it)
    {
      result.insert(std::make_pair(it->first, it->second));
    }
    datasets[newName] = result;
  }

  void processCommand(str command, std::istream& in, DatasetCollection& datasets)
  {
    if (command == "print")
    {
      std::string name;
      if (!(in >> name))
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }
      printDataset(name, datasets);
      return;
    }

    std::string newName;
    std::string name1;
    std::string name2;
    if (!(in >> newName >> name1 >> name2))
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    if (command == "complement")
    {
      complementDataset(newName, name1, name2, datasets);
    }
    else if (command == "intersect")
    {
      intersectDataset(newName, name1, name2, datasets);
    }
    else if (command == "union")
    {
      unionDataset(newName, name1, name2, datasets);
    }
    else
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}

