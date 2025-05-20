#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "BiTree.hpp"

using Dictionary = averenkov::Tree< int, std::string >;
using DictionaryStorage = averenkov::Tree< std::string, Dictionary >;
using str = const std::string&;

void loadDictionaries(str filename, DictionaryStorage& storage)
{
  std::ifstream file(filename);
  if (!file)
  {
    std::cerr << "file error\n";
    return;
  }

  std::string line;
  while (std::getline(file, line))
  {
    if (line.empty())
    {
      continue;
    }

    std::istringstream iss(line);
    std::string dictName;
    iss >> dictName;

    Dictionary dict;
    int key;
    std::string value;
    while (iss >> key >> value)
    {
      dict.push(key, value);
    }

    storage.push(dictName, dict);
  }
}

void printDictionary(const Dictionary& dict, str name)
{
  if (dict.empty())
  {
    std::cout << "<EMPTY>\n";
    return;
  }

  std::cout << name;
  for (auto it = dict.begin(); it != dict.end(); ++it)
  {
    std::cout << " " << it->first << " " << it->second;
  }

  std::cout << "\n";
}

void complement(DictionaryStorage& storage, str newName, str name1, str name2)
{
  auto dict1 = storage.find(name1);
  auto dict2 = storage.find(name2);

  if (dict1 == storage.end() || dict2 == storage.end())
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  Dictionary result;
  for (auto it = dict1->second.begin(); it != dict1->second.end(); ++it)
  {
    if (dict2->second.find(it->first) == dict2->second.end())
    {
      result.push(it->first, it->second);
    }
  }

  storage.push(newName, result);
}

void intersect(DictionaryStorage& storage, str newName, str name1, str name2)
{
  auto dict1 = storage.find(name1);
  auto dict2 = storage.find(name2);

  if (dict1 == storage.end() || dict2 == storage.end())
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  Dictionary result;
  for (auto it = dict1->second.begin(); it != dict1->second.end(); ++it)
  {
    if (dict2->second.find(it->first) != dict2->second.end())
    {
      result.push(it->first, it->second);
    }
  }

  storage.push(newName, result);
}

void unionDicts(DictionaryStorage& storage, str newName, str name1, str name2)
{
  auto dict1 = storage.find(name1);
  auto dict2 = storage.find(name2);

  if (dict1 == storage.end() || dict2 == storage.end())
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  Dictionary result;
  for (auto it = dict1->second.begin(); it != dict1->second.end(); ++it)
  {
    result.push(it->first, it->second);
  }
  for (auto it = dict2->second.begin(); it != dict2->second.end(); ++it)
  {
    if (result.find(it->first) == result.end())
    {
      result.push(it->first, it->second);
    }
  }

  storage.push(newName, result);
}

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " filename\n";
    return 1;
  }

  DictionaryStorage dictionaries;
  loadDictionaries(argv[1], dictionaries);

  std::string line;
  while (std::getline(std::cin, line))
  {
    std::istringstream iss(line);
    std::string command;
    iss >> command;

    if (command == "print")
    {
      std::string dictName;
      if (iss >> dictName)
      {
        auto dict = dictionaries.find(dictName);
        if (dict != dictionaries.end())
        {
          printDictionary(dict->second, dictName);
        }
        else
        {
          std::cout << "<INVALID COMMAND>\n";
        }
      }
      else
      {
        std::cout << "<INVALID COMMAND>\n";
      }
    }
    else if (command == "complement")
    {
      std::string newName, name1, name2;
      if (iss >> newName >> name1 >> name2)
      {
        complement(dictionaries, newName, name1, name2);
      }
      else
      {
        std::cout << "<INVALID COMMAND>\n";
      }
    }
    else if (command == "intersect")
    {
      std::string newName, name1, name2;
      if (iss >> newName >> name1 >> name2)
      {
        intersect(dictionaries, newName, name1, name2);
      }
      else
      {
        std::cout << "<INVALID COMMAND>\n";
      }
    }
    else if (command == "union")
    {
      std::string newName, name1, name2;
      if (iss >> newName >> name1 >> name2)
      {
        unionDicts(dictionaries, newName, name1, name2);
      }
      else
      {
        std::cout << "<INVALID COMMAND>\n";
      }
    }
    else
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
