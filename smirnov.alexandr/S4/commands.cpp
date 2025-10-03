#include "commands.hpp"
#include <limits>

void smirnov::print(const TreeOfTrees & trees, std::istream & in, std::ostream & out)
{
  std::string dictName;
  in >> dictName;
  const BasicTree & tree = trees.at(dictName);
  if (tree.empty())
  {
    out << "<EMPTY>\n";
    return;
  }
  out << dictName;
  for (BasicTree::const_iterator tree_it = tree.cbegin(); tree_it != tree.cend(); ++tree_it)
  {
    out << " " << tree_it->first << " " << tree_it->second;
  }
  out << "\n";
}

void smirnov::complement(TreeOfTrees & trees, std::istream & in)
{
  std::string newDict, firstDict, secondDict;
  in >> newDict >> firstDict >> secondDict;
  const BasicTree & first = trees.at(firstDict);
  const BasicTree & second = trees.at(secondDict);
  BasicTree result;
  for (BasicTree::const_iterator it = first.cbegin(); it != first.cend(); ++it)
  {
    if (second.find(it->first) == second.cend())
    {
      result[it->first] = it->second;
    }
  }
  trees[newDict] = result;
}

void smirnov::intersect(TreeOfTrees & trees, std::istream & in)
{
  std::string newDict, firstDict, secondDict;
  in >> newDict >> firstDict >> secondDict;
  const BasicTree & first = trees.at(firstDict);
  const BasicTree & second = trees.at(secondDict);
  BasicTree result;
  for (BasicTree::const_iterator it = first.cbegin(); it != first.cend(); ++it)
  {
    if (second.find(it->first) != second.cend())
    {
      result[it->first] = it->second;
    }
  }
  trees[newDict] = result;
}

void smirnov::unite(TreeOfTrees & trees, std::istream & in)
{
  std::string newDict, firstDict, secondDict;
  in >> newDict >> firstDict >> secondDict;
  const BasicTree & first = trees.at(firstDict);
  const BasicTree & second = trees.at(secondDict);
  BasicTree result = first;
  for (BasicTree::const_iterator it = second.cbegin(); it != second.cend(); ++it)
  {
    if (result.find(it->first) == result.cend())
    {
      result[it->first] = it->second;
    }
  }
  trees[newDict] = result;
}
