#include <iostream>
#include "datasetCommands.hpp"

int main(int argc, char **argv)
{
  using namespace abramov;
  if (argc != 2)
  {
    std::cerr << "Wrong parameters\n";
    return 1;
  }
  std::ifstream input(argv[1]);
  if (!input)
  {
    std::cerr << "Incorrect filename\n";
    return 1;
  }
  collections dicts = getDataSets(input);
  while (!std::cin.eof())
  {
    std::string s;
    std::cin >> s;
    try
    {
      doCommand(s, std::cin, dicts);
    }
    catch (const std::exception &)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}
