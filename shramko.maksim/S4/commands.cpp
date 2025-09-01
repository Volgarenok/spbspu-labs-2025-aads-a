#include "commands.hpp"
#include <limits>

void shramko::print(const TreeOfTrees & trees, std::istream & in, std::ostream & out)
{
  std::string dictName;
  in >> dictName;
  TreeOfTrees::const_iterator it = trees.find(dictName);
  if (it == trees.cend())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  const BasicTree & tree = it->second;
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

void shramko::complement(TreeOfTrees & trees, std::istream & in, std::ostream & out)
{
  std::string newDict, firstDict, secondDict;
  in >> newDict >> firstDict >> secondDict;
  TreeOfTrees::const_iterator first_it = trees.find(firstDict);
  TreeOfTrees::const_iterator second_it = trees.find(secondDict);
  if (first_it == trees.cend() || second_it == trees.cend())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  BasicTree result;
  for (BasicTree::const_iterator it = first_it->second.cbegin(); it != first_it->second.cend(); ++it)
  {
    if (second_it->second.find(it->first) == second_it->second.cend())
    {
      result[it->first] = it->second;
    }
  }
  trees[newDict] = result;
}

void shramko::intersect(TreeOfTrees & trees, std::istream & in, std::ostream & out)
{
  std::string newDict, firstDict, secondDict;
  in >> newDict >> firstDict >> secondDict;
  TreeOfTrees::const_iterator first_it = trees.find(firstDict);
  TreeOfTrees::const_iterator second_it = trees.find(secondDict);
  if (first_it == trees.cend() || second_it == trees.cend())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  BasicTree result;
  for (BasicTree::const_iterator it = first_it->second.cbegin(); it != first_it->second.cend(); ++it)
  {
    if (second_it->second.find(it->first) != second_it->second.cend())
    {
      result[it->first] = it->second;
    }
  }
  trees[newDict] = result;
}

void shramko::unite(TreeOfTrees & trees, std::istream & in, std::ostream & out)
{
  std::string newDict, firstDict, secondDict;
  in >> newDict >> firstDict >> secondDict;
  TreeOfTrees::const_iterator first_it = trees.find(firstDict);
  TreeOfTrees::const_iterator second_it = trees.find(secondDict);
  if (first_it == trees.cend() || second_it == trees.cend())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  BasicTree result = first_it->second;
  for (BasicTree::const_iterator it = second_it->second.cbegin(); it != second_it->second.cend(); ++it)
  {
    if (result.find(it->first) == result.cend())
    {
      result[it->first] = it->second;
    }
  }
  trees[newDict] = result;
}
