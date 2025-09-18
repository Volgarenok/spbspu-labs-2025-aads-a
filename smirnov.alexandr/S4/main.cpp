#include <iostream>
#include <fstream>
#include <limits>
#include <functional>
#include "inputTrees.hpp"
#include "commands.hpp"

int main(int argc, char** argv)
{
  using namespace smirnov;
  if (argc != 2)
  {
    std::cerr << "Invalid arguments\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "File open failed\n";
    return 1;
  }
  TreeOfTrees trees;
  inputTrees(trees, file);
  AvlTree< std::string, std::function< void() > > commands;
  commands["print"] = std::bind(print, std::cref(trees), std::ref(std::cin), std::ref(std::cout));
  commands["complement"] = std::bind(complement, std::ref(trees), std::ref(std::cin));
  commands["intersect"] = std::bind(intersect, std::ref(trees), std::ref(std::cin));
  commands["union"] = std::bind(unite, std::ref(trees), std::ref(std::cin));
  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      commands.at(command)();
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
