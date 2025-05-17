#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include <tree/tree.hpp>
#include "tree_manips.hpp"

namespace
{
  using TreeMap = demehin::Tree< size_t, std::string >;
  using MapOfTrees = demehin::Tree< std::string, TreeMap >;

  void inputTrees(std::istream& in, MapOfTrees& mapOfTrees)
  {
    while(!in.eof())
    {
      in.clear();
      TreeMap map;
      size_t key;
      std::string datasetName;
      in >> datasetName;
      while (in >> key)
      {
        std::string value;
        in >> value;
        map.insert(std::make_pair(key, value));
      }
      mapOfTrees.insert(std::make_pair(datasetName, map));
    }
  }
}

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    return 1;
  }

  std::ifstream file(argv[1]);
  MapOfTrees mapOfTrees;
  try
  {
    inputTrees(file, mapOfTrees);
  }
  catch (const std::exception&)
  {
    std::cout << "input error\n";
    return 1;
  }

  demehin::Tree< std::string, std::function< void() > > commands;
  commands["print"] = std::bind(demehin::print, std::ref(std::cout), std::ref(std::cin), std::cref(mapOfTrees));
  commands["complement"] = std::bind(demehin::makeComplement, std::ref(std::cin), std::ref(mapOfTrees));
  commands["intersect"] = std::bind(demehin::makeIntersect, std::ref(std::cin), std::ref(mapOfTrees));
  commands["union"] = std::bind(demehin::makeUnion, std::ref(std::cin), std::ref(mapOfTrees));

  std::string command;
  while (std::cin >> command)
  {
    try
    {
      commands.at(command)();
    }
    catch (const std::out_of_range&)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
