#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <limits>
#include "commands.hpp"
#include <tree.hpp>
#include <list.hpp>

namespace
{
  void input(std::istream & in, bocharov::dict_dict_t & dicts)
  {
    std::string dictName;
    while (in >> dictName)
    {
      bocharov::dict_t dict;
      std::string sln;
      while (in >> sln)
      {
        bocharov::List< std::string > ruswords;
        std::string rusword;
        while (in >> rusword)
        {
          ruswords.push_back(rusword);
          if (in.get() == '\n')
          {
            break;
          }
        }
        dict[sln] = ruswords;
        if (in.get() == '\n')
        {
          break;
        }
        else
        {
          in.unget();
        }
      }
      dicts[dictName] = dict;
    }
  }
}


int main(int argc, char * argv[])
{
  using namespace bocharov;
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <input_file>\n";
    return 1;
  }

  dict_dict_t dicts;
  if (argc > 1)
  {
    std::ifstream file(argv[1]);
    if (!file)
    {
      std::cout << "FILE ERROR\n";
      return 1;
    }
    input(file, dicts);
    file.close();
  }

  std::cout << "Loaded dictionaries:\n";
  for (const auto & dict_pair : dicts)
  {
    std::cout << "Dictionary: " << dict_pair.first << "\n";
    for (const auto & word_pair : dict_pair.second)
    {
      std::cout << "  " << word_pair.first << ": ";
      for (const auto & translation : word_pair.second)
      {
        std::cout << translation << " ";
      }
      std::cout << "\n";
    }
  }
  std::cout << "write --help to get a list of commands\n";

  bocharov::Tree<std::string, std::function<void()>> commands;
  commands["createdict"] = std::bind(createDict, std::ref(std::cin), std::ref(dicts));
  commands["deletedict"] = std::bind(deleteDict, std::ref(std::cin), std::ref(dicts));
  commands["printdict"] = std::bind(printDict, std::ref(std::cin), std::ref(std::cout), std::cref(dicts));
  commands["gettranslationsln"] = std::bind(getTranslationSln, std::ref(std::cin), std::ref(std::cout), std::cref(dicts));
  commands["gettranslationru"] = std::bind(getTranslationRu, std::ref(std::cin), std::ref(std::cout), std::cref(dicts));
  commands["deletesln"] = std::bind(deleteSln, std::ref(std::cin), std::ref(dicts));
  commands["addsln"] = std::bind(addSln, std::ref(std::cin), std::ref(dicts));
  commands["deleteru"] = std::bind(deleteRu, std::ref(std::cin), std::ref(dicts));
  commands["addru"] = std::bind(addRu, std::ref(std::cin), std::ref(dicts));
  commands["union"] = std::bind(makeUnion, std::ref(std::cin), std::ref(dicts));
  commands["rewritefile"] = std::bind(rewriteFile, std::ref(std::cin), std::cref(dicts));
  commands["writedicts"] = std::bind(writeDicts, std::ref(std::cin), std::cref(dicts));
  commands["mostcommon"] = std::bind(printMostCommon, std::ref(std::cin), std::ref(std::cout), std::cref(dicts));
  commands["rare"] = std::bind(rare, std::ref(std::cin), std::ref(dicts));
  commands["--help"] = std::bind(printHelp, std::ref(std::cout));

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      auto it = commands.find(cmd);
      if (it != commands.end())
      {
        it->second();
      }
      else
      {
        throw std::invalid_argument("Unknown command");
      }
    }
    catch (const std::exception & e)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

  return 0;
}
