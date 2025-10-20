#include <functional>
#include <iostream>
#include <fstream>
#include <string>
#include <tree.hpp>
#include "keysum.hpp"

int main(int argc, char* argv[])
{
  using namespace kushekbaev;
  if (argc != 3)
  {
    std::cerr << "Wrong number of elements!\n";
    return 1;
  }
  std::string command(argv[1]);
  std::string filename(argv[2]);
  Tree< long long int, std::string, std::less< long long int > > tree;
  std::ifstream in(filename);
  long long int key;
  std::string value;
  while (in >> key >> value)
  {
    tree.insert(std::make_pair(key, value));
  }
  if (tree.empty())
  {
    std::cout << "<EMPTY>\n";
    return 0;
  }
  using TraverseMode = KeySum(Tree< long long int, std::string, std::less< long long int > >::*)(KeySum);
  Tree< std::string, TraverseMode > commands;
  commands["ascending"] = &Tree< long long int, std::string, std::less< long long int > >::traverse_lnr;
  commands["descending"] = &Tree< long long int, std::string, std::less< long long int > >::traverse_rnl;
  commands["breadth"] = &Tree< long long int, std::string, std::less< long long int > >::traverse_breadth;
  try
  {
    kushekbaev::KeySum proc;
    auto mode = commands.at(command);
    proc = (tree.*mode)(proc);
    if (!proc.value.empty())
    {
      proc.value.pop_back();
    }
    std::cout << proc.sum << " " << proc.value << "\n";
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
  return 0;
}
