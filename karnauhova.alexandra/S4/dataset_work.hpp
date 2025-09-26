#ifndef DATASET_WORK_HPP
#define DATASET_WORK_HPP
#include <cstring>
#include <iostream>
#include <map>
#include "avl_tree.hpp"

namespace karnauhova
{
  using TreeKey = AvlTree< size_t, std::string >;
  using DataTree = AvlTree< std::string, TreeKey >;
  void printDataset(std::ostream& out, std::string named, DataTree& dataset);
  void complement(std::string name_new_tree, std::string name_tree1, std::string name_tree2, DataTree& set_trees);
  void intersect(std::string name_new_tree, std::string name_tree1, std::string name_tree2, DataTree& set_trees);
  void unionData(std::string name_new_tree, std::string name_tree1, std::string name_tree2, DataTree& set_trees);
}
#endif
