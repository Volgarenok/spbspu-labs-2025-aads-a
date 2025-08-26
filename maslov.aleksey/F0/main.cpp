#include <string>
#include <limits>
#include <fstream>
#include <iostream>
#include <functional>
#include <hashTable/hashTable.hpp>
#include <tree/tree.hpp>
#include "commands.hpp"

int main(int argc, char * argv[])
{
  using namespace maslov;
  HashTable< std::string, HashTable< std::string, int > > dicts;
  if (argc == 2)
  {
    if (std::string(argv[1]) == "--help")
    {
      printHelp(std::cout);
      return 0;
    }
    try
    {
      loadFile(std::string(argv[1]), dicts);
    }
    catch (const std::exception & e)
    {
      std::cerr << e.what() << '\n';
      return 1;
    }
  }
  BiTree< std::string, std::function< void() >, std::less< std::string > > cmds;
  cmds["createdict"] = std::bind(createDictionary, std::ref(std::cin), std::ref(dicts));
  cmds["showdicts"] = std::bind(showDictionary, std::ref(std::cout), std::cref(dicts));
  cmds["loadtext"] = std::bind(loadText, std::ref(std::cin), std::ref(dicts));
  cmds["union"] = std::bind(unionDictionary, std::ref(std::cin), std::ref(dicts));
  cmds["intersect"] = std::bind(intersectDictionary, std::ref(std::cin), std::ref(dicts));
  cmds["copy"] = std::bind(copyDictionary, std::ref(std::cin), std::ref(dicts));
  cmds["addword"] = std::bind(addWord, std::ref(std::cin), std::ref(dicts));
  cmds["size"] = std::bind(printSize, std::ref(std::cin), std::ref(std::cout), std::cref(dicts));
  cmds["cleanword"] = std::bind(cleanWord, std::ref(std::cin), std::ref(dicts));
  cmds["cleandict"] = std::bind(cleanDictionary, std::ref(std::cin), std::ref(dicts));
  cmds["top"] = std::bind(printTopRare, std::ref(std::cin), std::ref(std::cout), std::cref(dicts), "descending");
  cmds["rare"] = std::bind(printTopRare, std::ref(std::cin), std::ref(std::cout), std::cref(dicts), "ascending");
  cmds["frequency"] = std::bind(printFrequency, std::ref(std::cin), std::ref(std::cout), std::cref(dicts));
  cmds["wordrange"] = std::bind(createWordRange, std::ref(std::cin), std::ref(dicts));
  cmds["save"] = std::bind(saveDictionaries, std::ref(std::cin), std::cref(dicts));
  cmds["loadfile"] = std::bind(loadFileCommand, std::ref(std::cin), std::ref(dicts));
  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      cmds.at(command)();
    }
    catch (const std::out_of_range &)
    {
      std::cout << "<INVALID COMMAND>" << '\n';
    }
    catch (const std::exception & e)
    {
      std::cout << e.what() << '\n';
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
}
