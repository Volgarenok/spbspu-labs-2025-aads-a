#include <iostream>
#include <string>
#include "functions.h"

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cerr << "too many arguments!";
    return 1;
  }

  std::string mode = argv[1];
  std::string filename = argv[2];

  asafov::map< int, std::string > tree;

  if (parseFile(filename, tree) != 0)
  {
    return 1;
  }

  return traverseTree(mode, tree);
}
