#include <iostream>
#include <fstream>
#include <limits>
#include <cstddef>
#include <map>
#include <functional>
#include "AVLtree.hpp"
#include "commands.hpp"

namespace
{
  tkach::AvlTree< std::string, tkach::AvlTree< size_t, std::string > > inputDataSets(std::istream& in)
  {
    using namespace tkach;
    tkach::AvlTree< std::string, tkach::AvlTree< size_t, std::string > > fulldata;
    while(!in.eof())
    {
      AvlTree< size_t, std::string > temp;
      std::string name;
      in >> name;
      size_t key = 0;
      std::string value;
      char c = '\0';
      while(in.get(c) && c != '\n')
      {
        in >> key >> value;
        temp.insert(std::make_pair(key, value));
      }
      fulldata.insert(std::make_pair(name, std::move(temp)));
    }
    return fulldata;
  }
}

int main(const int argc, const char* const* const argv)
{
  using namespace tkach;
  if (argc != 2)
  {
    std::cerr << "Error: incorrect input\n";
    return 1;
  }
  std::fstream in(argv[1]);
  if (!in.is_open())
  {
    std::cerr << "File is not open\n";
    return 1;
  }
  AvlTree< std::string, AvlTree< size_t, std::string > > data = inputDataSets(in);
  std::map< std::string, std::function< void() > > cmds;
  cmds["print"] = std::bind(print, std::ref(std::cin), std::cref(data), std::ref(std::cout));
  cmds["intersect"] = std::bind(intersect, std::ref(std::cin), std::ref(data));
  cmds["complement"] = std::bind(complement, std::ref(std::cin), std::ref(data));
  cmds["union"] = std::bind(unionTree, std::ref(std::cin), std::ref(data));
  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      cmds.at(command)();
    }
    catch (const std::out_of_range&)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
    catch (const std::exception& e)
    {
      std::cerr << e.what() << "\n";
      return 1;
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
  return 0;
}
