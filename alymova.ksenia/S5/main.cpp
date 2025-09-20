#include <iostream>
#include <fstream>
#include <functional>
#include <tree/tree-2-3.hpp>
#include "tree-sum-functor.hpp"


template< class Tree, class F >
struct CallTraverse
{
  Tree tree;
  F (Tree::*traverse)(F);

  F operator()(F f)
  {
    return (tree.*traverse)(f);
  }
};

int main(int args, char** argv)
{
  using namespace alymova;
  using DatasetType = TwoThreeTree< int, std::string, std::less< int > >;

  if (args != 3)
  {
    std::cerr << "<INCORRECT ARGUMENTS>\n";
    return 1;
  }
  std::ifstream file;
  file.open(argv[2]);
  if (!file.is_open())
  {
    std::cerr << "<INCORRECT FILE>\n";
    return 1;
  }

  try
  {
    DatasetType dataset;
    while (!file.eof() && !file.fail())
    {
      int key;
      std::string value;
      if (file >> key >> value && !value.empty())
      {
        dataset[key] = value;
      }
    }
    if (file.fail() && !file.eof())
    {
      std::cerr << "<INCORRECT MAP>\n";
      return 1;
    }

    if (dataset.empty())
    {
      std::cout << "<EMPTY>\n";
      return 0;
    }
    std::string round = argv[1];
    TreeSum result;
    TwoThreeTree< std::string, std::function< TreeSum(TreeSum) >, std::less< std::string > > commandset{
      {"ascending", CallTraverse< DatasetType, TreeSum >{dataset, &DatasetType::traverse_lnr}},
      {"descending", CallTraverse< DatasetType, TreeSum >{dataset, &DatasetType::traverse_rnl}},
      {"breadth", CallTraverse< DatasetType, TreeSum >{dataset, &DatasetType::traverse_breadth}}
    };
    result = commandset.at(round)(result);
    std::cout << result << '\n';
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
}
