#include <functional>
#include <iostream>
#include <fstream>
#include <limits>
#include <tree/tree.hpp>
#include "accumulator.hpp"

namespace
{
  using Dataset = aleksandrov::Tree< long long int, std::string >;

  void readDataset(std::istream& in, Dataset& dest)
  {
    long long int key = 0;
    std::string value;
    while (in >> key >> value)
    {
      dest.insert({ key, value });
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

  Tree< long long int, std::string > dataset;
  Accumulator acc{};
  Tree< std::string, std::function< Accumulator() > > commands;
  commands["ascending"] = std::bind(&Dataset::traverseLNR< Accumulator >, std::addressof(dataset), acc);
  commands["descending"] = std::bind(&Dataset::traverseRNL< Accumulator >, std::addressof(dataset), acc);
  commands["breadth"] = std::bind(&Dataset::traverseBFS< Accumulator >, std::addressof(dataset), acc);

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
      acc = commands.at(command)();
      std::cout << acc.key << ' ' << acc.value << '\n';
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

