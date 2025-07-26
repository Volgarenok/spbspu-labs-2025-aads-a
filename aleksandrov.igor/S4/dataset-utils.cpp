#include "dataset-utils.hpp"
#include <iostream>

namespace aleksandrov
{
  void readDatasets(std::istream& in, Datasets& datasets)
  {
    while (!in.eof())
    {
      Dataset dataset;
      int key = 0;
      std::string name;
      if (in >> name)
      {
        while (in >> key)
        {
          std::string value;
          in >> value;
          dataset.insert({ key, value });
        }
        datasets.insert({ name, dataset });
      }
      if (in.fail())
      {
        in.clear(in.rdstate() ^ std::ios::failbit);
      }
    }
  }

  void printDataset(std::ostream& out, const std::string& name, const Datasets& datasets)
  {
    auto it = datasets.find(name);
    if (it == datasets.cend())
    {
      out << "<INVALID COMMAND>";
      return;
    }

    const Dataset& dataset = it->second;
    if (dataset.empty())
    {
      out << "<EMPTY>";
      return;
    }

    out << name;
    for (auto it = dataset.cbegin(); it != dataset.cend(); ++it)
    {
      out << ' ' << it->first << ' ' << it->second;
    }
  }

  void complement(const std::string& name, const Dataset& a, const Dataset& b, Datasets& datasets)
  {
    Dataset result;
    for (auto it = a.cbegin(); it != a.cend(); ++it)
    {
      if (b.find(it->first) == b.cend())
      {
        result.insert({ it->first, it->second });
      }
    }
    datasets[name] = result;
  }

  void intersect(const std::string& name, const Dataset& a, const Dataset& b, Datasets& datasets)
  {
    Dataset result;
    for (auto it = a.cbegin(); it != a.cend(); ++it)
    {
      if (b.find(it->first) != b.cend())
      {
        result.insert({ it->first, it->second });
      }
    }
    datasets[name] = result;
  }

  void unionCmd(const std::string& name, const Dataset& a, const Dataset& b, Datasets& datasets)
  {
    Dataset result = a;
    for (auto it = b.cbegin(); it != b.cend(); ++it)
    {
      result.insert({ it->first, it->second });
    }
    datasets[name] = result;
  }

  void processCommand(const std::string& command, std::istream& in, std::ostream& out, Datasets& datasets)
  {
    if (command == "print")
    {
      std::string name;
      if (!(in >> name))
      {
        out << "<INVALID COMMAND>\n";
        return;
      }
      printDataset(out, name, datasets);
      out << '\n';
      return;
    }

    std::string newName;
    std::string name1;
    std::string name2;
    if (!(in >> newName >> name1 >> name2))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    auto it1 = datasets.find(name1);
    auto it2 = datasets.find(name2);

    if (it1 == datasets.end() || it2 == datasets.end())
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    const Dataset& a = it1->second;
    const Dataset& b = it2->second;

    if (command == "complement")
    {
      complement(newName, a, b, datasets);
    }
    else if (command == "intersect")
    {
      intersect(newName, a, b, datasets);
    }
    else if (command == "union")
    {
      unionCmd(newName, a, b, datasets);
    }
    else
    {
      out << "<INVALID COMMAND>\n";
    }
  }
}

