#include <iostream>
#include <fstream>
#include <avl_tree.hpp>
#include "traverse_info_handler.hpp"

namespace
{
  int checkArguments(const int argc);

  int checkArguments(const int argc)
  {
    if (argc == 3)
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }
}

int main(const int argc, const char * const * argv)
{
  using namespace petrov;
  if (!checkArguments(argc))
  {
    std::cerr << "ERROR: Invalid number of command line arguments";
    std::cerr << '\n';
    return 1;
  }
  AVLTree< long long, std::string > tree;
  std::ifstream in(argv[2]);
  while (!in.eof())
  {
    long long key = 0;
    std::string val;
    if (in >> key >> val)
    {
      tree.insert({ key, val });
    }
  }
  SumUpKeysDisplayValues sum_and_display;
  std::string traverse_order(argv[1]);
  try
  {
    if (tree.empty())
    {
      std::cout << "<EMPTY>";
    }
    else if (traverse_order == "ascending")
    {
      SumUpKeysDisplayValues new_sum_and_display = tree.traverse_lnr(sum_and_display);
      std::cout << new_sum_and_display.result << ' ';
      new_sum_and_display.displayValuesInTraverseOrder(std::cout);
    }
    else if (traverse_order == "descending")
    {
      SumUpKeysDisplayValues new_sum_and_display = tree.traverse_rnl(sum_and_display);
      std::cout << new_sum_and_display.result << ' ';
      new_sum_and_display.displayValuesInTraverseOrder(std::cout);
    }
    else if (traverse_order == "breadth")
    {
      SumUpKeysDisplayValues new_sum_and_display = tree.traverse_breadth(sum_and_display);
      std::cout << new_sum_and_display.result << ' ';
      new_sum_and_display.displayValuesInTraverseOrder(std::cout);
    }
    else
    {
      std::cerr << "ERROR: Invalid argument";
      std::cerr << '\n';
      return 2;
    }
  }
  catch(const std::out_of_range & e)
  {
    std::cerr << e.what() << '\n';
    return 3;
  }
  std::cout << '\n';
}
