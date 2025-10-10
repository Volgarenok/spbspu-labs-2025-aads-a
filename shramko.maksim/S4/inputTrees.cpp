#include "inputTrees.hpp"

void shramko::inputTrees(TreeOfTrees & trees, std::istream & input)
{
  std::string dictName;
  int key;
  std::string value;
  while (input >> dictName)
  {
    BasicTree tree;
    while (input >> key >> value)
    {
      tree[key] = value;
    }
    input.clear();
    trees[dictName] = tree;
  }
}
