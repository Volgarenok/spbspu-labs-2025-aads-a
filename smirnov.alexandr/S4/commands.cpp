#include "commands.hpp"
#include <limits>

void smirnov::print(const TreeOfTrees & trees, std::istream & in, std::ostream & out)
{
  std::string dictName;
  in >> dictName;
  TreeOfTrees::const_iterator it = trees.find(dictName);
  if (it == trees.end())
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
  for (BasicTree::const_iterator tree_it = tree.begin(); tree_it != tree.end(); ++tree_it)
  {
    out << " " << tree_it->first << " " << tree_it->second;
  }
  out << "\n";
}

void smirnov::complement(TreeOfTrees & trees, std::istream & in, std::ostream & out)
{
  std::string newDict, firstDict, secondDict;
  in >> newDict >> firstDict >> secondDict;
  TreeOfTrees::iterator first_it = trees.find(firstDict);
  TreeOfTrees::iterator second_it = trees.find(secondDict);
  if (first_it == trees.end() || second_it == trees.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  BasicTree result;
  for (BasicTree::const_iterator it = first_it->second.begin(); it != first_it->second.end(); ++it)
  {
    if (second_it->second.find(it->first) == second_it->second.end())
    {
      result[it->first] = it->second;
    }
  }
  trees[newDict] = result;
}

void smirnov::intersect(TreeOfTrees & trees, std::istream & in, std::ostream & out)
{
  std::string newDict, firstDict, secondDict;
  in >> newDict >> firstDict >> secondDict;
  TreeOfTrees::iterator first_it = trees.find(firstDict);
  TreeOfTrees::iterator second_it = trees.find(secondDict);
  if (first_it == trees.end() || second_it == trees.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  BasicTree result;
  for (BasicTree::const_iterator it = first_it->second.begin(); it != first_it->second.end(); ++it)
  {
    if (second_it->second.find(it->first) != second_it->second.end())
    {
      result[it->first] = it->second;
    }
  }
  trees[newDict] = result;
}

void smirnov::unite(TreeOfTrees & trees, std::istream & in, std::ostream & out)
{
  std::string newDict, firstDict, secondDict;
  in >> newDict >> firstDict >> secondDict;
  TreeOfTrees::iterator first_it = trees.find(firstDict);
  TreeOfTrees::iterator second_it = trees.find(secondDict);
  if (first_it == trees.end() || second_it == trees.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  BasicTree result = first_it->second;
  for (BasicTree::const_iterator it = second_it->second.begin(); it != second_it->second.end(); ++it)
  {
    if (result.find(it->first) == result.end())
    {
      result[it->first] = it->second;
    }
  }
  trees[newDict] = result;
}
