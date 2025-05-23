#include <iostream>
#include <fstream>
#include <limits>
#include "map_operations.hpp"
#include "tree/definition.hpp"

int main(int argc, char** argv)
{
  using namespace maslevtsov;

  if (argc != 2) {
    std::cerr << "<INVALID PARAMETERS>\n";
    return 1;
  }
  std::ifstream fin(argv[1]);
  if (!fin) {
    std::cerr << "<INVALID DATA FILE>\n";
    return 1;
  }
  setmap_t data_map;
  try {
    input_setmap(fin, data_map);
  } catch (const std::bad_alloc&) {
    std::cerr << "<INPUT ERROR>\n";
    return 1;
  }
  commands_t commands;
  try {
    fill_with_commands(commands);
  } catch (const std::bad_alloc&) {
    std::cerr << "<COMMANDS REGISTRATION ERROR>\n";
    return 1;
  }
  std::string command;
  while (std::cin) {
    std::cin >> command;
    if (std::cin.eof()) {
      break;
    }
    try {
      commands.at(command)(std::cin, data_map);
    } catch (...) {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
