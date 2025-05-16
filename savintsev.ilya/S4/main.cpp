#include <iostream>
#include <string>
#include "dataset.h"

int main(int argc, char ** argv)
{
  if (argc != 2)
  {
    std::cerr << "ERROR: No file\n";
    return 1;
  }

  DatasetCollection datasets;
  datasets = savintsev::load_dataset_from(argv[1]);

  if (datasets.empty())
  {
    std::cerr << "ERROR: Invalid file\n";
    return 1;
  }

  DatasetCommands datasets_commands = savintsev::register_commands();

  std::string command;
  while (std::cin)
  {
    std::cin >> command;
    if (!std::cin)
    {
      break;
    }
    try
    {
      datasets_commands.at(command)(datasets);
    }
    catch (const std::exception & e)
    {
      std::cout << "<INVALID COMMAND>\n";
      while (std::cin.peek() != '\n')
      {
        std::cin >> command;
      }
    }
  }
}
