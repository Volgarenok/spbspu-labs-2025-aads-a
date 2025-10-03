#include <iostream>
#include <string>
#include <fstream>
#include <tree/avl_tree.hpp>
#include "key_sum.hpp"

namespace
{
  void inputTree(karnauhova::AvlTree< int, std::string >& tree, std::istream& in)
  {
    int key;
    std::string val;
    while (in >> key >> val)
    {
      tree.insert(std::make_pair(key, val));
    }
  }
}

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cerr << "Error: Invalid count arguments\n";
    return 1;
  }
  std::string way = argv[1];
  if (way != "ascending" && way != "descending" && way != "breadth")
  {
    std::cerr << "Error: Invalid workaround option. Use 'ascending', 'descending' or 'breadth'\n";
    return 1;
  }
  karnauhova::AvlTree< int, std::string > tree;
  std::ifstream file(argv[2]);
  karnauhova::KeySum res;
  try
  {
    inputTree(tree, file);
    if (!file.eof())
    {
      std::cerr << "Invalid input\n";
      return 1;
    }
    if (tree.empty())
    {
      std::cout << "<EMPTY>\n";
      return 0;
    }
    if (way == "ascending")
    {
       res = tree.traverse_lnr(res);
    }
    else if (way == "descending")
    {
       res = tree.traverse_rnl(res);
    }
    else if (way == "breadth")
    {
       res = tree.traverse_breadth(res);
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  std::cout << res.result_ << " " << res.itInput_ << "\n";
}
