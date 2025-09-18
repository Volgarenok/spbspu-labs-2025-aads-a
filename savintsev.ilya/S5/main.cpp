#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <functional>
#include "cmds.hpp"
#include <two-three-tree.h>

namespace savintsev
{
  struct KeyAdder
  {
    void operator()(const std::pair< long long, std::string > val)
    {
      if (val.first > 0 && result > std::numeric_limits< long long >::max() - val.first)
      {
        throw std::overflow_error("Key sum overflow (positive)");
      }
      else if (val.first < 0 && result < std::numeric_limits< long long >::min() - val.first)
      {
        throw std::overflow_error("Key sum overflow (negative)");
      }

      result += val.first;
      values += " " + val.second;
    }

    long long result = 0;
    std::string values;
  };
}

int main(int argc, char * argv[])
{
  if (argc != 3)
  {
    std::cerr << "ERROR: Invalid number of arguments\n";
    return 1;
  }

  std::string traverse_mode(argv[1]);
  std::string filename(argv[2]);

  using namespace savintsev;

  using DataTree = TwoThreeTree< long long, std::string >;

  DataTree tree;

  std::ifstream file(filename);
  if (!file)
  {
    std::cerr << "ERROR: Can't open file\n";
    return 1;
  }

  long long key;
  std::string value;
  while (file >> key >> value)
  {
    tree.insert({key, value});
  }

  if (tree.empty())
  {
    std::cout << "<EMPTY>\n";
    return 0;
  }

  TwoThreeTree< std::string, std::function< void(KeyAdder &) > > commands;

  using namespace std::placeholders;

  commands["ascending"] = std::bind(ascending< KeyAdder & >, std::ref(tree), _1);
  commands["descending"] = std::bind(descending< KeyAdder & >, std::ref(tree), _1);
  commands["breadth"] = std::bind(breadth< KeyAdder & >, std::ref(tree), _1);

  try
  {
    KeyAdder summator;

    commands.at(traverse_mode)(summator);

    std::cout << summator.result << summator.values << "\n";
  }
  catch (const std::overflow_error & e)
  {
    std::cerr << "ERROR: " << e.what() << "\n";
    return 1;
  }
  catch (const std::out_of_range & e)
  {
    std::cerr << "ERROR: Traverse doesn't exist (Invalid argument)\n";
    return 1;
  }
  catch (const std::logic_error & e)
  {
    std::cout << "<EMPTY>\n";
    return 0;
  }
}
