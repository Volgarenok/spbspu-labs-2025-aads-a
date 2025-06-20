#include <iostream>
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

    try
    {
      if (cmd == commands.end())
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
      cmd->second(dictionaries, args);
    }
    catch (const std::out_of_range& e)
    {
      std::cout << "<EMPTY>\n";
      return 0;
    }
    catch (const std::invalid_argument& e)
    {
      std::cout << "<INVALID COMMAND>\n";
      return 0;
    }
  }

  return 0;
}
