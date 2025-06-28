#include "commands.hpp"
#include <fstream>
#include <iostream>

void maslov::printCommand(std::istream & in, std::ostream & out, const Dictionaries & dicts)
{
  std::string dictName;
  in >> dictName;
  auto it = dicts.find(dictName);
  if (it == dicts.cend())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  auto dict = it->second;
  if (dict.empty())
  {
    throw std::logic_error("<EMPTY>");
  }
  out << dictName;
  for (auto it = dict.cbegin(); it != dict.cend(); ++it)
  {
    out << ' ' << it->first << ' ' << it->second;
  }
  out << '\n';
}

void maslov::complementCommand(std::istream & in, Dictionaries & dicts)
{
  std::string resultName, dictName1, dictName2;
  in >> resultName >> dictName1 >> dictName2;
  auto it1 = dicts.find(dictName1);
  auto it2 = dicts.find(dictName2);
  if (it1 == dicts.end() || it2 == dicts.end())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  auto dict1 = it1->second;
  auto dict2 = it2->second;
  BiTree< int, std::string, std::less< int > > result;
  for (auto it = dict1.cbegin(); it != dict1.cend(); ++it)
  {
    if (dict2.find(it->first) == dict2.end())
    {
      result.push(it->first, it->second);
    }
  }
  dicts.push(resultName, result);
}

void maslov::intersectCommand(std::istream & in, Dictionaries & dicts)
{
  std::string resultName, dictName1, dictName2;
  in >> resultName >> dictName1 >> dictName2;
  auto it1 = dicts.find(dictName1);
  auto it2 = dicts.find(dictName2);
  if (it1 == dicts.end() || it2 == dicts.end())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  auto dict1 = it1->second;
  auto dict2 = it2->second;
  BiTree< int, std::string, std::less< int > > result;
  for (auto it = dict1.cbegin(); it != dict1.cend(); ++it)
  {
    if (dict2.find(it->first) != dict2.end())
    {
      result.push(it->first, it->second);
    }
  }
  dicts.push(resultName, result);
}

void maslov::unionCommand(std::istream & in, Dictionaries & dicts)
{
  std::string resultName, dictName1, dictName2;
  in >> resultName >> dictName1 >> dictName2;
  auto it1 = dicts.find(dictName1);
  auto it2 = dicts.find(dictName2);
  if (it1 == dicts.end() || it2 == dicts.end())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  auto dict1 = it1->second;
  auto dict2 = it2->second;
  BiTree< int, std::string, std::less< int > > result = dict1;
  for (auto it = dict2.cbegin(); it != dict2.cend(); ++it)
  {
    if (result.find(it->first) == result.end())
    {
      result.push(it->first, it->second);
    }
  }
  dicts.push(resultName, result);
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
