#include <fstream>
#include <limits>
#include <stdexcept>
#include <functional>
#include "commands.hpp"
#include "inpututil.hpp"

using dataset_t = kushekbaev::Tree< size_t, std::string >;
using dict_t = kushekbaev::Tree< std::string, dataset_t >;


int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "<WRONG NUMBER OF ARGUMENTS>";
    return 1;
  }
  std::ifstream in(argv[1]);
  dict_t dictionary = kushekbaev::readInputFromFile(in);
  kushekbaev::Tree< std::string, std::function< void() > > commands;
  commands["print"] = std::bind(kushekbaev::print, std::ref(std::cout), std::ref(std::cin), std::cref(dictionary));
  commands["complement"] = std::bind(kushekbaev::complement, std::ref(std::cin), std::ref(dictionary));
  commands["intersect"] = std::bind(kushekbaev::intersect, std::ref(std::cin), std::ref(dictionary));
  commands["union"] = std::bind(kushekbaev::unification, std::ref(std::cin), std::ref(dictionary));
  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      commands.at(command)();
    }
    catch (const std::out_of_range& e)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
    catch (const std::exception& e)
    {
      std::cout << e.what() << "\n";
      return 1;
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
  return 0;
}
