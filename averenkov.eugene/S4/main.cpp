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
      dict.insert(std::pair< const int, std::string >(key, value));
    }

    storage.insert(std::pair< const std::string, Dictionary >(dictName, dict));
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
      result.insert(std::pair< const int, std::string >(it->first, it->second));
    }
  }

  storage.insert(std::pair< const std::string, Dictionary >(newName, result));
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
      result.insert(std::pair< const int, std::string >(it->first, it->second));
    }
  }

  storage.insert(std::pair< const std::string, Dictionary >(newName, result));
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

  Dictionary result = dict1->second;
  for (auto it = dict2->second.begin(); it != dict2->second.end(); ++it)
  {
    if (result.find(it->first) == result.end())
    {
      result.insert(std::pair< const int, std::string >(it->first, it->second));
    }
  }

  storage.insert(std::pair< const std::string, Dictionary >(newName, result));
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
          std::cout << "<EMPTY>\n";
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
/*#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <BiTree.hpp>

namespace averenkov
{
  using Dictionary = Tree< int, std::string >;
  using DictionaryStorage = Tree< std::string, Dictionary >;
  using DicFunc = std::function< void(DictionaryStorage&, const std::string&) >;
  using DicIter = Iterator< std::string, Dictionary, std::less< std::string > >;
  using DicPair = std::pair< DicIter, DicIter >;
  using CommandTree = Tree< std::string, DicFunc >;

  void loadDictionaries(const std::string& filename, DictionaryStorage& storage)
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

  void printDictionary(DictionaryStorage& storage, const std::string& dictName)
  {
    auto dict = storage.find(dictName);
    if (dict == storage.end() || dict->second.empty())
    {
      std::cout << "<EMPTY>\n";
      return;
    }

    std::cout << dictName;
    for (auto it = dict->second.begin(); it != dict->second.end(); ++it)
    {
      std::cout << " " << it->first << " " << it->second;
    }
    std::cout << "\n";
  }

  DicPair parse(DictionaryStorage& storage, const std::string& args)
  {
    size_t pos = 0;
    size_t space1 = args.find(' ', pos);
    size_t space2 = args.find(' ', space1 + 1);
    std::string newName = args.substr(pos, space1);
    std::string name1 = args.substr(space1 + 1, space2 - space1 - 1);
    std::string name2 = args.substr(space2 + 1);
    auto dict1 = storage.find(name1);
    auto dict2 = storage.find(name2);
    if (dict1 == storage.end() || dict2 == storage.end())
    {
      throw;
    }
    return { dict1, dict2 };
  }

  void complement(DictionaryStorage& storage, const std::string& args)
  {
    std::string newName = args.substr(0, args.find(' ', 0) + 1);
    auto Dicts = parse(storage, args);
    Dictionary result;
    for (auto it = Dicts.first->second.begin(); it != Dicts.first->second.end(); ++it)
    {
      if (Dicts.second->second.find(it->first) == Dicts.second->second.end())
      {
        result.push(it->first, it->second);
      }
    }
    storage.push(newName, result);
  }

  void intersect(DictionaryStorage& storage, const std::string& args)
  {
    std::string newName = args.substr(0, args.find(' ', 0) + 1);
    auto Dicts = parse(storage, args);
    Dictionary result;
    for (auto it = Dicts.first->second.begin(); it != Dicts.first->second.end(); ++it)
    {
      if (Dicts.second->second.find(it->first) != Dicts.second->second.end())
      {
        result.push(it->first, it->second);
      }
    }
    storage.push(newName, result);
  }

  void unionDicts(DictionaryStorage& storage, const std::string& args)
  {
    std::string newName = args.substr(0, args.find(' ', 0) + 1);
    auto Dicts = parse(storage, args);
    Dictionary result;
    for (auto it = Dicts.first->second.begin(); it != Dicts.first->second.end(); ++it)
    {
      result.push(it->first, it->second);
    }
    for (auto it = Dicts.second->second.begin(); it != Dicts.second->second.end(); ++it)
    {
      if (result.find(it->first) == result.end())
      {
        result.push(it->first, it->second);
      }
    }
    storage.push(newName, result);
  }

  void commandsInit(CommandTree& commands)
  {
    commands["print"] = std::bind(printDictionary, std::placeholders::_1, std::placeholders::_2);
    commands["complement"] = std::bind(complement, std::placeholders::_1, std::placeholders::_2);
    commands["intersect"] = std::bind(intersect, std::placeholders::_1, std::placeholders::_2);
    commands["union"] = std::bind(unionDicts, std::placeholders::_1, std::placeholders::_2);
  }
}

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " filename\n";
    return 1;
  }

  averenkov::DictionaryStorage dictionaries;
  averenkov::loadDictionaries(argv[1], dictionaries);

  averenkov::CommandTree commands;
  averenkov::commandsInit(commands);

  std::string line;
  while (std::getline(std::cin, line))
  {
    if (line.empty())
    {
      continue;
    }

    size_t space_pos = line.find(' ');
    std::string command = line.substr(0, space_pos);
    std::string args = space_pos == std::string::npos ? "" : line.substr(space_pos + 1);

    auto cmd = commands.find(command);
    if (cmd != commands.end())
    {
      try
      {
        cmd->second(dictionaries, args);
      }
      catch (...)
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
*/
