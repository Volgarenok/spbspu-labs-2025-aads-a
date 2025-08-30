#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <tree.hpp>
#include "commands.hpp"

namespace
{
  void inputDicts(std::istream & in, mozhegova::BiTree< std::string, mozhegova::BiTree< int, std::string > > & dicts)
  {
    while (!in.eof())
    {
      in.clear();
      mozhegova::BiTree< int, std::string > temp;
      std::string name;
      in >> name;
      int key = 0;
      std::string value;
      while ((in >> key) && (in >> value))
      {
        temp.insert(std::make_pair(key, value));
      }
      dicts.insert(std::make_pair(name, temp));
    }
  }
}

int main(int argc, char * argv[])
{
  using namespace mozhegova;
  if (argc != 2)
  {
    std::cerr << "invalid arguments\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "file isn't open\n";
    return 1;
  }
  BiTree< std::string, BiTree< int, std::string > > dicts;
  inputDicts(file, dicts);
  BiTree< std::string, std::function< void() > > cmds;
  cmds["print"] = std::bind(print, std::ref(std::cin), std::ref(std::cout), std::cref(dicts));
  cmds["complement"] = std::bind(complement, std::ref(std::cin), std::ref(dicts));
  cmds["intersect"] = std::bind(intersect, std::ref(std::cin), std::ref(dicts));
  cmds["union"] = std::bind(unionCmd, std::ref(std::cin), std::ref(dicts));
  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      cmds.at(command)();
    }
    catch (const std::out_of_range &)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
    catch (const std::exception & e)
    {
      std::cerr << e.what() << "\n";
      return 1;
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
}
