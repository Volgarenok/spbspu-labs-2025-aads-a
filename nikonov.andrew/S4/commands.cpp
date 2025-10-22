#include "commands.hpp"
#include <limits>

namespace nikonov
{
  void inputTrees(TreeOfTrees & trees, std::istream & input)
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
  void print(TreeOfTrees& trees, std::istream& in, std::ostream& out)
  {
    std::string dictName;
    in >> dictName;
    const BasicTree& tree = trees.at(dictName);
    if (tree.empty())
    {
      out << "<EMPTY>\n";
      return;
    }
    out << dictName;
    for (auto tree_it = tree.cbegin(); tree_it != tree.cend(); ++tree_it)
    {
      out << " " << tree_it->first << " " << tree_it->second;
    }
    out << "\n";
  }

  void complement(TreeOfTrees& trees, std::istream& in, std::ostream&)
  {
    std::string newDict, firstDict, secondDict;
    in >> newDict >> firstDict >> secondDict;
    const BasicTree& first = trees.at(firstDict);
    const BasicTree& second = trees.at(secondDict);
    BasicTree result;
    for (auto it = first.cbegin(); it != first.cend(); ++it)
    {
      if (second.find(it->first) == second.cend())
      {
        result[it->first] = it->second;
      }
    }
    trees[newDict] = result;
  }

  void intersect(TreeOfTrees& trees, std::istream& in, std::ostream&)
  {
    std::string newDict, firstDict, secondDict;
    in >> newDict >> firstDict >> secondDict;
    const BasicTree& first = trees.at(firstDict);
    const BasicTree& second = trees.at(secondDict);
    BasicTree result;
    for (auto it = first.cbegin(); it != first.cend(); ++it)
    {
      if (second.find(it->first) != second.cend())
      {
        result[it->first] = it->second;
      }
    }
    trees[newDict] = result;
  }

  void unite(TreeOfTrees& trees, std::istream& in, std::ostream&)
  {
    std::string newDict, firstDict, secondDict;
    in >> newDict >> firstDict >> secondDict;
    const BasicTree& first = trees.at(firstDict);
    const BasicTree& second = trees.at(secondDict);
    BasicTree result = first;
    for (auto it = second.cbegin(); it != second.cend(); ++it)
    {
      if (result.find(it->first) == result.cend())
      {
        result[it->first] = it->second;
      }
    }
    trees[newDict] = result;
  }
}
