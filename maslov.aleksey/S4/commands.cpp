#include "commands.hpp"
#include <fstream>
#include <iostream>
#include <limits>

namespace
{
  void printCommand(std::istream & in, const maslov::Dictionaries & dicts)
  {
    std::string dictName;
    in >> dictName;
    auto it = dicts.find(dictName);
    if (it == dicts.cend())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
    auto dict = it->second;
    if (dict.empty())
    {
      std::cout << "<EMPTY>\n";
      return;
    }
    std::cout << dictName;
    for (auto it = dict.cbegin(); it != dict.cend(); ++it)
    {
      std::cout << ' ' << it->first << ' ' << it->second;
    }
    std::cout << '\n';
  }

  void complementCommand(std::istream & in, maslov::Dictionaries & dicts)
  {
    std::string resultName, dictName1, dictName2;
    in >> resultName >> dictName1 >> dictName2;
    auto it1 = dicts.find(dictName1);
    auto it2 = dicts.find(dictName2);
    if (it1 == dicts.end() || it2 == dicts.end())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
    auto dict1 = it1->second;
    auto dict2 = it2->second;
    maslov::BiTree< int, std::string, std::less< int > > result;
    for (auto it = dict1.cbegin(); it != dict1.cend(); ++it)
    {
      if (dict2.find(it->first) == dict2.end())
      {
        result.push(it->first, it->second);
      }
    }
    dicts.push(resultName, result);
  }

  void intersectCommand(std::istream & in, maslov::Dictionaries & dicts)
  {
    std::string resultName, dictName1, dictName2;
    in >> resultName >> dictName1 >> dictName2;
    auto it1 = dicts.find(dictName1);
    auto it2 = dicts.find(dictName2);
    if (it1 == dicts.end() || it2 == dicts.end())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
    auto dict1 = it1->second;
    auto dict2 = it2->second;
    maslov::BiTree< int, std::string, std::less< int > > result;
    for (auto it = dict1.cbegin(); it != dict1.cend(); ++it)
    {
      if (dict2.find(it->first) != dict2.end())
      {
        result.push(it->first, it->second);
      }
    }
    dicts.push(resultName, result);
  }

  void unionCommand(std::istream & in, maslov::Dictionaries & dicts)
  {
    std::string resultName, dictName1, dictName2;
    in >> resultName >> dictName1 >> dictName2;
    auto it1 = dicts.find(dictName1);
    auto it2 = dicts.find(dictName2);
    if (it1 == dicts.end() || it2 == dicts.end())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
    auto dict1 = it1->second;
    auto dict2 = it2->second;
    maslov::BiTree< int, std::string, std::less< int > > result = dict1;
    for (auto it = dict2.cbegin(); it != dict2.cend(); ++it)
    {
      if (result.find(it->first) == result.end())
      {
        result.push(it->first, it->second);
      }
    }
    dicts.push(resultName, result);
  }
}

void maslov::processCommand(std::istream & in, const std::string & command, Dictionaries & dicts)
{
  if (command == "print")
  {
    printCommand(in, dicts);
  }
  else if (command == "complement")
  {
    complementCommand(in, dicts);
  }
  else if (command == "intersect")
  {
    intersectCommand(in, dicts);
  }
  else if (command == "union")
  {
    unionCommand(in, dicts);
  }
  else
  {
    std::cout << "<INVALID COMMAND>\n";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
}

void maslov::inputFile(const std::string & filename, Dictionaries & dicts)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("ERROR: can't open file");
  }
  std::string dataset;
  while (file >> dataset)
  {
    BiTree< int, std::string, std::less< int > > dict;
    int key;
    std::string value;
    while ((file >> key) && (file >> value))
    {
      dict.push(key, value);
    }
    dicts.push(dataset, dict);
    file.clear();
  }
}
