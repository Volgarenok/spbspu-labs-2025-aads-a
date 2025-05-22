#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include "inputDict.hpp"
#include "commands.hpp"

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cout << "empty parameter\n";
    return 1;
  }
  std::string filename = argv[1];
  std::ifstream inputFile(filename);
  if (!inputFile.is_open())
  {
    std::cout << "error open file: " << filename << "\n";
    return 1;
  }
  std::map< std::string, std::map < int, std::string > > mainDict;
  finaev::inDictionaries(inputFile, mainDict);
  inputFile.close();
  while (true)
  {
    std::string command = "";
    std::cin >> command;
    try
    {
      if (std::cin.eof())
      {
        break;
      }
      else if (command == "print")
      {
        finaev::printCMD(std::cout, mainDict);
      }
      else if (command == "complement")
      {
        finaev::complementCMD(mainDict);
      }
      else if (command == "intersect")
      {
        finaev::intersectCMD(mainDict);
      }
      else if (command == "union")
      {
        finaev::unionCMD(mainDict);
      }
      else
      {
        std::cout << "<INVALID COMMAND>\n";
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
    }
    catch(...)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
