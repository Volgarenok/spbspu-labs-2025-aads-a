#include <iostream>
#include <fstream>
#include <string>
#include <BiTree.hpp>

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

    size_t space_pos = line.find(' ');
    if (space_pos == std::string::npos)
    {
      continue;
    }

    std::string dictName = line.substr(0, space_pos);
    Dictionary dict;
    size_t pos = space_pos + 1;
    while (pos < line.length())
    {
      size_t next_space = line.find(' ', pos);
      if (next_space == std::string::npos)
      {
        break;
      }
      std::string key_str = line.substr(pos, next_space - pos);
      pos = next_space + 1;
      next_space = line.find(' ', pos);
      if (next_space == std::string::npos)
      {
        next_space = line.length();
      }
      std::string value = line.substr(pos, next_space - pos);
      pos = next_space + 1;
      try
      {
        int key = std::stoi(key_str);
        dict.push(key, value);
      }
      catch (...)
      {
        continue;
      }
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
    std::cerr << "Error\n";
    return 1;
  }

  DictionaryStorage dictionaries;
  loadDictionaries(argv[1], dictionaries);

  std::string line;
  while (std::getline(std::cin, line))
  {
    if (line.empty())
    {
      return 1;
    }

    size_t space_pos = line.find(' ');
    std::string command = line.substr(0, space_pos);

    if (command == "print")
    {
      if (space_pos == std::string::npos)
      {
        std::cout << "<INVALID COMMAND>\n";
        return 1;
      }
      std::string dictName = line.substr(space_pos + 1);
      auto dict = dictionaries.find(dictName);
      if (dict != dictionaries.end())
      {
        printDictionary(dict->second, dictName);
      }
      else
      {
        std::cout << "<INVALID COMMAND>\n";
        return 1;
      }
    }
    else if (command == "complement" || command == "intersect" || command == "union")
    {
      size_t pos1 = space_pos + 1;
      size_t pos2 = line.find(' ', pos1);
      size_t pos3 = line.find(' ', pos2 + 1);
      if (pos1 == std::string::npos || pos2 == std::string::npos || pos3 == std::string::npos)
      {
        std::cout << "<INVALID COMMAND>\n";
        return 1;
      }
      std::string newName = line.substr(pos1, pos2 - pos1);
      std::string name1 = line.substr(pos2 + 1, pos3 - pos2 - 1);
      std::string name2 = line.substr(pos3 + 1);
      auto dict1 = dictionaries.find(name1);
      auto dict2 = dictionaries.find(name2);
      if (dict1 == dictionaries.end() || dict2 == dictionaries.end())
      {
        std::cout << "<INVALID COMMAND>\n";
        return 1;
      }
      if (command == "complement")
      {
        complement(dictionaries, newName, name1, name2);
      }
      else if (command == "intersect")
      {
        intersect(dictionaries, newName, name1, name2);
      }
      else
      {
        unionDicts(dictionaries, newName, name1, name2);
      }
    }
    else
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
