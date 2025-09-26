#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>
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
  using str = const std::string&;

  void loadDictionaries(const std::string& filename, DictionaryStorage& storage)
  {
    std::ifstream file(filename);
    if (!file)
    {
      throw std::invalid_argument("file error");
    }
    std::string line;
    while (std::getline(file, line))
    {
      if (line.empty())
      {
        continue;
      }
      size_t start = line.find_first_not_of(" ");
      size_t end = line.find_last_not_of(" ");
      if (start == std::string::npos)
      {
        continue;
      }
      line = line.substr(start, end - start + 1);
      size_t space_pos = line.find(' ');
      std::string dictName;
      Dictionary dict;
      if (space_pos == std::string::npos)
      {
        dictName = line;
      }
      else
      {
        dictName = line.substr(0, space_pos);
        size_t pos = space_pos + 1;
        while (pos < line.length())
        {
          while (pos < line.length() && line[pos] == ' ')
          {
            pos++;
          }
          if (pos >= line.length())
          {
            break;
          }
          size_t key_end = line.find(' ', pos);
          if (key_end == std::string::npos)
          {
            break;
          }
          std::string key_str = line.substr(pos, key_end - pos);
          pos = key_end + 1;
          while (pos < line.length() && line[pos] == ' ')
          {
            pos++;
          }
          if (pos >= line.length())
          {
            break;
          }
          size_t value_end = line.find(' ', pos);
          if (value_end == std::string::npos)
          {
            value_end = line.length();
          }
          std::string value = line.substr(pos, value_end - pos);
          pos = value_end + 1;
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
      }
      storage.push(dictName, dict);
    }
  }

  void printDictionary(DictionaryStorage& storage, const std::string& dictName)
  {
    auto dict = storage.find(dictName);
    if (dict == storage.end())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    if (dict->second.empty())
    {
      throw std::out_of_range("<EMPTY>");
    }
    std::cout << dictName;
    for (auto it = dict->second.begin(); it != dict->second.end(); ++it)
    {
      std::cout << " " << it->first << " " << it->second;
    }
    std::cout << "\n";
  }

  void complement(DictionaryStorage& storage, const std::string& args)
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
      throw std::invalid_argument("<INVALID COMMAND>");
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

  void intersect(DictionaryStorage& storage, const std::string& args)
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
      throw std::invalid_argument("<INVALID COMMAND>");
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

  void unionDicts(DictionaryStorage& storage, const std::string& args)
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
      throw std::invalid_argument("<INVALID COMMAND>");
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

  void commandsInit(CommandTree& commands)
  {
    commands["print"] = printDictionary;
    commands["complement"] = complement;
    commands["intersect"] = intersect;
    commands["union"] = unionDicts;
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

    try
    {
      if (cmd == commands.end())
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
      cmd->second(dictionaries, args);
    }
    catch (const std::exception& e)
    {
      std::cout << e.what() << "\n";
    }
  }

  return 0;
}
