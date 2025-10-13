#include <iostream>
#include <fstream>
#include <functional>
#include <binaryTree.hpp>
#include "keySummer.hpp"

namespace
{
  using biTree = krylov::BiTree< int, std::string, std::less< int > >;
  void callAscending(const biTree & tree, krylov::KeySummator& result)
  {
    result = tree.traverseLnr(result);
  }

  void callDescending(const biTree & tree, krylov::KeySummator& result)
  {
    result = tree.traverseRnl(result);
  }

  void callBreadth(const biTree& tree, krylov::KeySummator& result)
  {
    result = tree.traverseBreadth(result);
  }
}

int main(int argc, char** argv)
{
  using namespace krylov;
  if (argc != 3)
  {
    std::cerr << "Wrong arguments\n";
    return 1;
  }
  std::ifstream file(argv[2]);
  if (!file)
  {
    std::cerr << "Wrong filename\n";
  }
  std::string value;
  int key = 0;
  BiTree< int, std::string, std::less< int > > biTree;
  while ((file >> key >> value) && !file.eof())
  {
    biTree[key] = value;
  }
  if (!file.eof())
  {
    std::cerr << "Incorrect input!\n";
    return 1;
  }
  if (biTree.empty())
  {
    std::cout << "<EMPTY>\n";
    return 0;
  }
  BiTree< std::string, std::function< void(KeySummator&) >, std::less< std::string > > commands;
  using namespace std::placeholders;
  commands["ascending"] = std::bind(callAscending, std::cref(biTree), _1);
  commands["descending"] = std::bind(callDescending, std::cref(biTree), _1);
  commands["breadth"] = std::bind(callBreadth, std::cref(biTree), _1);
  try
  {
    KeySummator result;
    commands.at(argv[1])(result);
    std::cout << result.keySum << " " << result.values << "\n";
  }
  catch (const std::out_of_range & e)
  {
    std::cerr << "<INVALID COMMAND>\n";
    return 1;
  }
}
