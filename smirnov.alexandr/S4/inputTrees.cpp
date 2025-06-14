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
      tree.insert({ key, value });
    }
    trees.insert({ dictName, tree });
    input.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
}
