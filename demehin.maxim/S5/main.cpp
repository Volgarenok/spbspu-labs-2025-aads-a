#include <iostream>
#include <fstream>
#include <tree/tree.hpp>
#include "key_summ.hpp"

namespace
{
  using Map = demehin::Tree< int, std::string >;

  void inputTree(std::istream& in, Map& tree)
  {
    int key;
    std::string val;
    while (in >> key >> val)
    {
      tree.insert(std::make_pair(key, val));
    }
  }

  void traverseLnr(const Map& tree, demehin::KeySumm& res)
  {
    res = tree.traverse_lnr(res);
  }

  void traverseRnl(const Map& tree, demehin::KeySumm& res)
  {
    res = tree.traverse_rnl(res);
  }

  void traverseBreadth(const Map& tree, demehin::KeySumm& res)
  {
    res = tree.traverse_breadth(res);
  }
}

int main(int argc, char* argv[])
{
  using namespace demehin;
  using namespace std::placeholders;

  if (argc != 3)
  {
    return 1;
  }

  std::ifstream file(argv[2]);
  Map tree;
  KeySumm res;
  Tree< std::string, std::function< void(Map&, KeySumm&) > > cmds;

  cmds["ascending"] = std::bind(traverseLnr, _1, _2);
  cmds["descending"] = std::bind(traverseRnl, _1, _2);
  cmds["breadth"] = std::bind(traverseBreadth, _1, _2);

  try
  {
    inputTree(file, tree);
    if (!file.eof())
    {
      std::cerr << "incorrect input\n";
      return 1;
    }
    if (tree.empty())
    {
      std::cout << "<EMPTY>\n";
      return 0;
    }
    cmds.at(argv[1])(tree, res);
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }

  std::cout << res.val_res << " " << res.str_res << "\n";
}
