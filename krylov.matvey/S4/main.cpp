#include <iostream>
#include <functional>
#include <limits>
#include <map>
#include <binaryTree.hpp>
#include "dataCommands.hpp"

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cerr << "ERROR: wrong arguments\n";
    return 1;
  }
  krylov::BiTree< std::string, krylov::BiTree< int, std::string, std::less< int > >, std::less< std::string > > dicts;
  krylov::BiTree< std::string, std::function< void() >, std::less< std::string > > commands;
  commands["print"] = std::bind(krylov::printComm, std::ref(std::cin), std::ref(std::cout), std::cref(dicts));
  commands["complement"] = std::bind(krylov::complementComm, std::ref(std::cin), std::ref(dicts));
  commands["intersect"] = std::bind(krylov::intersectComm, std::ref(std::cin), std::ref(dicts));
  commands["union"] = std::bind(krylov::unionComm, std::ref(std::cin), std::ref(dicts));
  krylov::inputFile(argv[1], dicts);
  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      commands.at(command)();
    }
    catch (const std::out_of_range&)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
    catch (const std::exception& e)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << e.what() <<"\n";
    }
  }
}
