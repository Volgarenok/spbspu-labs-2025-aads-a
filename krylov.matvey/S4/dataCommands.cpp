#include "dataCommands.hpp"
#include <fstream>
#include <iostream>

void krylov::printComm(std::istream& in, std::ostream& out, const Dictionary& dicts)
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
    out << " " << it->first << " " << it->second;
  }
  out << "\n";
}

void krylov::complementComm(std::istream& in, Dictionary& dicts)
{
  std::string resultName, dictName1, dictName2;
  in >> resultName >> dictName1 >> dictName2;
  auto it1 = dicts.find(dictName1);
  auto it2 = dicts.find(dictName2);
  if (it1 == dicts.cend() || it2 == dicts.cend())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  auto dict1 = it1->second;
  auto dict2 = it2->second;
  BiTree< int, std::string, std::less< int > > result;
  for (auto it = dict1.cbegin(); it != dict1.cend(); ++it)
  {
    if (dict2.find(it->first) == dict2.cend())
    {
      result[it->first] = it->second;
    }
  }
  dicts[resultName] = result;
}

void krylov::intersectComm(std::istream& in, Dictionary& dicts)
{
  std::string resultName, dictName1, dictName2;
  in >> resultName >> dictName1 >> dictName2;
  auto it1 = dicts.find(dictName1);
  auto it2 = dicts.find(dictName2);
  if (it1 == dicts.cend() || it2 == dicts.cend())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  auto dict1 = it1->second;
  auto dict2 = it2->second;
  BiTree< int, std::string, std::less< int > > result;
  for (auto it = dict1.cbegin(); it != dict1.cend(); ++it)
  {
    if (dict2.find(it->first) != dict2.cend())
    {
      result[it->first] = it->second;
    }
  }
  dicts[resultName] = result;
}

void krylov::unionComm(std::istream& in, Dictionary& dicts)
{
  std::string resultName, dictName1, dictName2;
  in >> resultName >> dictName1 >> dictName2;
  auto it1 = dicts.find(dictName1);
  auto it2 = dicts.find(dictName2);
  if (it1 == dicts.cend() || it2 == dicts.cend())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  auto dict1 = it1->second;
  auto dict2 = it2->second;
  BiTree< int, std::string, std::less< int > > result = dict1;
  for (auto it = dict2.cbegin(); it != dict2.cend(); ++it)
  {
    if (result.find(it->first) == result.cend())
    {
      result[it->first] = it->second;
    }
  }
  dicts[resultName] = result;
}

void krylov::inputFile(const std::string& filename, Dictionary& dicts)
{
  std::ifstream file(filename);
  std::string dataSet;
  while (file >> dataSet)
  {
    BiTree< int, std::string, std::less< int > > dict;
    int key;
    std::string value;
    while ((file >> key) && (file >> value))
    {
      dict[key] = value;
    }
    dicts[dataSet] = dict;
    file.clear();
  }
}
