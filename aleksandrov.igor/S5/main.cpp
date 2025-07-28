#include <functional>
#include <iostream>
#include <fstream>
#include <map>

int main(int argc, char* argv[])
{
  if (argc != 2)
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

  std::map< std::string, std::function< void() > > commands;

  try
  {
    commands.at(command)();
    std::cout << '\n';
  }
  catch (const std::out_of_range&)
  {
    std::cout << "<INVALID COMMAND>\n";
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 2;
  }
}

