#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <algorithm>
#include "inputDict.hpp"
#include "commands.hpp"

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cout << "Empty parameter\n";
    return 1;
  }
  std::string filename = argv[1];
  std::ifstream inputFile(filename);
  if (!inputFile.is_open())
  {
    std::cout << "Error open file: " << filename << "\n";
    return 1;
  }
  lebedev::AVLtree< std::string, lebedev::AVLtree< int, std::string > > dictOfDicts;
  try
  {
    lebedev::inputDicts(inputFile, dictOfDicts);
  }
  catch (...)
  {
    std::cout << "error input!\n";
    return 1;
  }
  lebedev::AVLtree< std::string, std::function< void() > > commands;
  commands["print"] = std::bind(lebedev::printCMD, std::ref(std::cout), std::cref(dictOfDicts));
  commands["complement"] = std::bind(lebedev::complementCMD, std::ref(dictOfDicts));
  commands["intersect"] = std::bind(lebedev::intersectCMD, std::ref(dictOfDicts));
  commands["union"] = std::bind(lebedev::unionCMD, std::ref(dictOfDicts));
  std::string command = "";
  while (std::cin >> command)
  {
    try
    {
      auto func = commands.find(command);
      if (func != commands.end())
      {
        func->second();
      }
      else
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
