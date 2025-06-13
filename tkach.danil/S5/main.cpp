#include <iostream>
#include <fstream>
#include <functional>
#include <map>
#include <AVLtree.hpp>
#include "key_summ.hpp"

int main(const int argc, const char* const* const argv)
{
  using namespace tkach;
  if (argc != 3)
  {
    std::cerr << "Error: incorrect input\n";
    return 1;
  }
  std::fstream in(argv[2]);
  if (!in.is_open())
  {
    std::cerr << "File is not open\n";
    return 1;
  }
  int key = 0;
  std::string value;
  AvlTree< int, std::string > tree;
  while (in >> key >> value)
  {
    tree.insert(std::make_pair(key, value));
  }
  if (!in.eof())
  {
    std::cerr << "Error: incorrect input\n";
    return 1;
  }
  if (tree.empty())
  {
    std::cout << "<EMPTY>\n";
    return 0;
  }
  std::map< std::string, std::function< KeySumm(KeySumm) > > cmds;
  using avltree = AvlTree< int, std::string >;
  using namespace std::placeholders;
  using traverse_method = KeySumm (avltree::*)(KeySumm);
  cmds["ascending"] = std::bind(static_cast< traverse_method >(&avltree::traverseLnr< KeySumm >), std::addressof(tree), _1);
  cmds["descending"] = std::bind(static_cast< traverse_method >(&avltree::traverseRnl< KeySumm >), std::addressof(tree), _1);
  cmds["breadth"] = std::bind(static_cast< traverse_method >(&avltree::traverseBreadth< KeySumm >), std::addressof(tree), _1);
  std::string command = argv[1];
  KeySumm res;
  try
  {
    res = cmds.at(command)(res);
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
  std::cout << res.key_sum << " " << res.values << "\n";
  return 0;
}
