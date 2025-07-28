#include <functional>
#include <iostream>
#include <fstream>
#include <tree/tree.hpp>

int main(int argc, char* argv[])
{
  using namespace aleksandrov;

  if (argc != 3)
  {
    std::cerr << "ERROR: Incorrect arguments!\n";
    return 1;
  }

  std::string command(argv[1]);

  std::ifstream file(argv[2]);
  if (!file)
  {
    std::cerr << "ERROR: Incorrect file!\n";
    return 1;
  }

  Tree< std::string, std::function< void() > > commands;

  try
  {
    commands.at(command)();
    std::cout << '\n';
  }
  catch (const std::out_of_range&)
  {
    std::cerr << "ERROR: Incorrect command!\n";
    return 1;
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 2;
  }
}

