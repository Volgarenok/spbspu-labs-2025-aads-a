#include "dataset_work.hpp"

void karnauhova::printDataset(std::ostream& out, std::string named, DataTree& dataset)
{
  TreeKey& tree = dataset.at(named);
  if (tree.empty())
  {
    out << "<EMPTY>\n";
    return;
  }
  out << named;
  for (auto it = tree.cbegin(); it != tree.cend(); it++)
  {
    std::pair< size_t, std::string > now = *it;
    out << " " << now.first << " " << now.second;
  }
  out << "\n";
}

void karnauhova::complement(std::string name_new_tree, std::string name_tree1, std::string name_tree2, DataTree& set_trees)
{
  TreeKey new_tree;
  TreeKey& tree1 = set_trees.at(name_tree1);
  TreeKey& tree2 = set_trees.at(name_tree2);
  for (auto it = tree1.cbegin(); it != tree1.cend(); it++)
  {
    if (tree2.find(it->first) == tree2.end())
    {
      std::pair< size_t, std::string > now = *it;
      new_tree.insert(now);
    }
  }
  set_trees[name_new_tree] = new_tree;
}

void karnauhova::intersect(std::string name_new_tree, std::string name_tree1, std::string name_tree2, DataTree& set_trees)
{
  TreeKey new_tree;
  TreeKey& tree1 = set_trees.at(name_tree1);
  TreeKey& tree2 = set_trees.at(name_tree2);
  for (auto it = tree1.cbegin(); it != tree1.cend(); it++)
  {
    if (tree2.find(it->first) != tree2.end())
    {
      std::pair< size_t, std::string > now = *it;
      new_tree.insert(now);
    }
  }
  set_trees[name_new_tree] = new_tree;
}

void karnauhova::unionData(std::string name_new_tree, std::string name_tree1, std::string name_tree2, DataTree& set_trees)
{
  TreeKey new_tree;
  TreeKey& tree1 = set_trees.at(name_tree1);
  TreeKey& tree2 = set_trees.at(name_tree2);
  for (auto it = tree1.cbegin(); it != tree1.cend(); it++)
  {
    if (tree2.find(it->first) == tree2.end())
    {
      std::pair< size_t, std::string > now = *it;
      new_tree.insert(now);
    }
  }
  for (auto it = tree2.cbegin(); it != tree2.cend(); it++)
  {
    if (tree1.find(it->first) == tree1.end())
    {
      std::pair< size_t, std::string > now = *it;
      new_tree.insert(now);
    }
  }
  for (auto it = tree1.cbegin(); it != tree1.cend(); it++)
  {
    if (tree2.find(it->first) != tree2.end())
    {
      std::pair< size_t, std::string > now = *it;
      new_tree.insert(now);
    }
  }
  set_trees[name_new_tree] = new_tree;
}
