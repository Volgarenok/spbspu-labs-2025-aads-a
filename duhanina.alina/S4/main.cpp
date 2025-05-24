#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include "tree.hpp"


using Dictionary = duhanina::Tree< size_t, std::string, std::less< size_t > >;
using DictionaryStorage = duhanina::Tree< std::string, Dictionary, std::less< std::string > >;

namespace
{
  std::string readNextToken(const std::string& str, size_t& pos)
  {
    std::string token;
    while (pos < str.size() && str[pos] != ' ')
    {
      token += str[pos++];
    }
    pos++;
    return token;
  }

  bool loadData(const std::string& filename, DictionaryStorage& storage)
  {
    std::ifstream file(filename);
    if (!file)
    {
      std::cerr << "Error file\n";
      return false;
    }
    std::string line;
    while (std::getline(file, line))
    {
      if (line.empty())
      {
        continue;
      }
      size_t pos = 0;
      std::string name = readNextToken(line, pos);
      Dictionary dict;
      while (pos < line.size())
      {
        std::string key_str = readNextToken(line, pos);
        std::string value = readNextToken(line, pos);
        if (!key_str.empty() && !value.empty())
        {
          size_t key = std::stoul(key_str);
          dict.push(key, value);
        }
      }
      if (!name.empty())
      {
        storage.push(name, dict);
      }
    }
    return true;
  }

  void printDict(const DictionaryStorage& storage, const std::string& name)
  {
    try
    {
      Dictionary dict = storage.get(name);
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
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  void processCommand(DictionaryStorage& storage, const std::string& cmd, const std::string& args)
  {
    if (cmd == "print")
    {
      printDict(storage, args);
    }
    else if (cmd == "complement" || cmd == "intersect" || cmd == "union")
    {
      size_t pos = 0;
      std::string newName = readNextToken(args, pos);
      std::string name1 = readNextToken(args, pos);
      std::string name2 = readNextToken(args, pos);
      try
      {
        if (cmd == "complement")
        {
          Dictionary dict1 = storage.get(name1);
          Dictionary dict2 = storage.get(name2);
          Dictionary result;
          for (auto it = dict1.begin(); it != dict1.end(); ++it)
          {
            if (!dict2.count(it->first))
            {
              result.push(it->first, it->second);
            }
          }
          storage.push(newName, result);
        }
        else if (cmd == "intersect")
        {
          Dictionary dict1 = storage.get(name1);
          Dictionary dict2 = storage.get(name2);
          Dictionary result;

          for (auto it = dict1.begin(); it != dict1.end(); ++it)
          {
            if (dict2.count(it->first))
            {
              result.push(it->first, it->second);
            }
          }
          storage.push(newName, result);
        }
        else if (cmd == "union")
        {
          Dictionary dict1 = storage.get(name1);
          Dictionary dict2 = storage.get(name2);
          Dictionary result = dict1;

          for (auto it = dict2.begin(); it != dict2.end(); ++it)
          {
            if (!result.count(it->first))
            {
              result.push(it->first, it->second);
            }
          }
          storage.push(newName, result);
        }
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
}

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Error parameters\n";
    return 1;
  }
  DictionaryStorage storage;
  if (!loadData(argv[1], storage))
  {
    return 1;
  }
  std::string line;
  while (std::getline(std::cin, line))
  {
    if (line.empty())
    {
      continue;
    }
    size_t pos = 0;
    std::string cmd = readNextToken(line, pos);
    std::string args = line.substr(pos);
    processCommand(storage, cmd, args);
  }
  return 0;
}
