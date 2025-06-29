#include <iostream>
#include <functional>
#include <limits>
#include "commands.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "ERROR: wrong arguments\n";
    return 1;
  }
  maslov::Dictionaries dicts;
  maslov::BiTree< std::string, std::function< void() >, std::less< std::string > > cmds;
  cmds["print"] = std::bind(maslov::printCommand, std::ref(std::cin), std::ref(std::cout), std::cref(dicts));
  cmds["complement"] = std::bind(maslov::complementCommand, std::ref(std::cin), std::ref(dicts));
  cmds["intersect"] = std::bind(maslov::intersectCommand, std::ref(std::cin), std::ref(dicts));
  cmds["union"] = std::bind(maslov::unionCommand, std::ref(std::cin), std::ref(dicts));
  inputFile(argv[1], dicts);
  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      cmds.at(command)();
    }
    catch (const std::out_of_range &)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
    catch (const std::exception & e)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << e.what() << '\n';
    }
  }
}
