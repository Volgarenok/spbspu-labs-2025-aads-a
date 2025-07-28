#include <functional>
#include <iostream>
#include <fstream>
#include <map>

int main(int argc, char* argv[])
{
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

  std::map< std::string, std::function< void() > > commands;
  commands["ascending"] = std::bind(ascending,);
  commands["descending"] = std::bind(descending,);
  commands["breadth"] = std::bind(breadth,);

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

