#include <limits>
#include <algorithm>
#include <string>
#include <iostream>
#include <tree.hpp>
#include "input_output_dictionaries.hpp"
#include "command_processing.hpp"

int main(int argc, char** argv)
{
  using namespace lanovenko;
  if (argc != 2)
  {
    std::cerr << "<INVALID PARAMETRS COUNT>\n";
    return 1;
  }
  std::ifstream input(argv[1]);
  map_t dictionaries{};
  getDictionaries(input, dictionaries);

  commands_t commands;
  try
  {
    commands["print"] = printDictionary;
    commands["complement"] = complement;
    commands["union"] = merge;
    commands["intersect"] = intersect;
  }
  catch (...)
  {
    std::cerr << "<OPERATION ERROR>";
    return 1;
  }

  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      commands.at(command)(dictionaries);
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
