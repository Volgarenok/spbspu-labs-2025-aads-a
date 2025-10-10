#include "commands.hpp"
#include <stdexcept>

namespace shramko
{
  void invalidOutput(std::ostream& out)
  {
    out << "<INVALID COMMAND>\n";
  }

  bool isValidDict(const TreeOfTrees& trees, const std::string& name)
  {
    return trees.find(name) != trees.cend();
  }

  void print(const TreeOfTrees& trees, std::istream& in, std::ostream& out)
  {
    std::string dictName;
    in >> dictName;
    if (!isValidDict(trees, dictName))
    {
      invalidOutput(out);
      return;
    }
    TreeOfTrees::const_iterator it = trees.find(dictName);
    const BasicTree& tree = it->second;
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

  void complement(TreeOfTrees& trees, std::istream& in, std::ostream& out)
  {
    std::string newDict, firstDict, secondDict;
    in >> newDict >> firstDict >> secondDict;
    if (!isValidDict(trees, firstDict) || !isValidDict(trees, secondDict))
    {
      invalidOutput(out);
      return;
    }
    TreeOfTrees::const_iterator first_it = trees.find(firstDict);
    TreeOfTrees::const_iterator second_it = trees.find(secondDict);
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

  void intersect(TreeOfTrees& trees, std::istream& in, std::ostream& out)
  {
    std::string newDict, firstDict, secondDict;
    in >> newDict >> firstDict >> secondDict;
    if (!isValidDict(trees, firstDict) || !isValidDict(trees, secondDict))
    {
      invalidOutput(out);
      return;
    }
    TreeOfTrees::const_iterator first_it = trees.find(firstDict);
    TreeOfTrees::const_iterator second_it = trees.find(secondDict);
    BasicTree result;
    for (BasicTree::const_iterator it = first_it->second.cbegin(); it != first_it->second.cend(); ++it)
    {
      auto sec_it = second_it->second.find(it->first);
      if (sec_it != second_it->second.cend())
      {
        result[it->first] = it->second;
      }
    }
    trees[newDict] = result;
  }

  void unite(TreeOfTrees& trees, std::istream& in, std::ostream& out)
  {
    std::string newDict, firstDict, secondDict;
    in >> newDict >> firstDict >> secondDict;
    if (!isValidDict(trees, firstDict) || !isValidDict(trees, secondDict))
    {
      invalidOutput(out);
      return;
    }
    TreeOfTrees::const_iterator first_it = trees.find(firstDict);
    TreeOfTrees::const_iterator second_it = trees.find(secondDict);
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
}
