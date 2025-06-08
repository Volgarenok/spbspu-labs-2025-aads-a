#include <iostream>
#include <limits>
#include <string>
#include "dataset.h"

int main(int argc, char ** argv)
{
  if (argc != 2)
  {
    std::cerr << "ERROR: No file\n";
    return 1;
  }

  using namespace savintsev;

  DatasetCollection datasets;
  datasets = load_dataset_from(argv[1]);

  if (datasets.empty())
  {
    std::cerr << "ERROR: Invalid file\n";
    return 1;
  }

  DatasetCommands datasets_commands;
  datasets_commands.insert({"print", print_dataset});
  datasets_commands.insert({"complement", complement_datasets});
  datasets_commands.insert({"intersect", intersect_datasets});
  datasets_commands.insert({"union", union_datasets});

  for (std::string command; std::cin >> command;)
  {
    try
    {
      datasets_commands.at(command)(datasets);
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
