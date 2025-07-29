#include <functional>
#include <iostream>
#include <fstream>
#include <tree/tree.hpp>

namespace
{
  using namespace aleksandrov;

  void readDataset(std::istream& in, Tree< int, std::string >& dest)
  {
    int key = 0;
    while (!(in >> key).eof())
    {
      std::string value;
      if (in >> value)
      {
        dest.insert({ key, value });
      }
      if (in.fail())
      {
        in.clear(in.rdstate() ^ std::ios::failbit);
      }
    }
  }
}

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

  Tree< int, std::string > dataset;
  Tree< std::string, std::function< void() > > commands;

  try
  {
    readDataset(file, dataset);

    if (dataset.empty())
    {
      std::cout << "<EMPTY>\n";
      return 0;
    }
    else
    {
      commands.at(command)();
      std::cout << '\n';
    }
  }
  catch (const std::out_of_range&)
  {
    std::cerr << "ERROR: Incorrect command!\n";
    return 1;
  }
  catch (const std::bad_alloc&)
  {
    std::cerr << "ERROR: Out of memory!\n";
    return 2;
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 2;
  }
}

