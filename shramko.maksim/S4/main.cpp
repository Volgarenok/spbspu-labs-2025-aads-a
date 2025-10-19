#include <iostream>
#include <fstream>
#include <limits>
#include "inputTrees.hpp"
#include "commandProcessor.hpp"

int main(int argc, char** argv)
{
  using namespace shramko;
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
  std::string command;
  while (std::cin >> command)
  {
    processCommand(trees, command, std::cin, std::cout);
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
  return 0;
}
