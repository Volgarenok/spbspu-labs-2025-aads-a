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

  void traverseTree(Map& tree, demehin::KeySumm& res, const std::string& command)
  {
    if (command == "ascending")
    {
      res = tree.traverse_lnr(res);
    }
    else if (command == "descending")
    {
      res = tree.traverse_rnl(res);
    }
    else if (command == "breadth")
    {
      res = tree.traverse_breadth(res);
    }
    else
    {
      throw std::logic_error("unknown command");
    }
  }

}

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    return 1;
  }

  std::ifstream file(argv[2]);
  Map tree;
  demehin::KeySumm res;
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
    traverseTree(tree, res, argv[1]);
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }

  std::cout << res.val_res << " " << res.str_res << "\n";
}
