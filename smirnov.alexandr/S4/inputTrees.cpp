#include "inputTrees.hpp"
#include <limits>

void smirnov::inputTrees(TreeOfTrees & trees, std::istream & input)
{
  std::string dictName;
  int key;
  std::string value;
  while (input >> dictName)
  {
    BasicTree tree;
    while (input.peek() != '\n' && input >> key >> value)
    {
      tree[key] = value;
    }
    trees[dictName] = tree;
    input.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
}
