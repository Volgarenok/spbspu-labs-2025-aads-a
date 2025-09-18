#include <iostream>
#include <fstream>
#include <functional>
#include <tree.hpp>
#include "key_summ.hpp"

namespace
{
  using Tree = mozhegova::BiTree< int, std::string >;
  void doAscending(const Tree & tree, mozhegova::KeySumm & res)
  {
    res = tree.traverseLnr(res);
  }

  void doDescending(const Tree & tree, mozhegova::KeySumm & res)
  {
    res = tree.traverseRnl(res);
  }

  void doBreadth(const Tree & tree, mozhegova::KeySumm & res)
  {
    res = tree.traverseBreadth(res);
  }
}

int main(int argc, char * argv[])
{
  using namespace mozhegova;
  if (argc != 3)
  {
    std::cerr << "wrong arguments\n";
    return 1;
  }
  std::ifstream file(argv[2]);
  if (!file.is_open())
  {
    std::cerr << "file is not open\n";
    return 1;
  }
  int key = 0;
  std::string value;
  BiTree< int, std::string > tree;
  while (file >> key >> value)
  {
    tree.insert(std::make_pair(key, value));
  }
  if (!file.eof())
  {
    std::cerr << "invalid input\n";
    return 1;
  }
  if (tree.empty())
  {
    std::cout << "<EMPTY>\n";
    return 0;
  }
  BiTree< std::string, std::function< void(KeySumm &) > > cmds;
  using namespace std::placeholders;
  cmds["ascending"] = std::bind(doAscending, std::cref(tree), _1);
  cmds["descending"] = std::bind(doDescending, std::cref(tree), _1);
  cmds["breadth"] = std::bind(doBreadth, std::cref(tree), _1);
  std::string command = argv[1];
  KeySumm res;
  try
  {
    cmds.at(command)(res);
  }
  catch (const std::exception & e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  std::cout << res.keySum << ' ' << res.values << '\n';
}
