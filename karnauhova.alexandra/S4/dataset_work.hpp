#ifndef DATASET_WORK_HPP
#define DATASET_WORK_HPP
#include <cstring>
#include <iostream>
#include <map>
#include "bitree.hpp"
namespace karnauhova
{
  using TreeKey = std::map< size_t, std::string >;
  using DataTree = std::map< std::string, TreeKey >;
  void print_dataset(std::ostream& out, std::string named, DataTree& dataset);
  void complement(std::string name_new_tree, std::string name_tree1, std::string name_tree2, DataTree& set_trees);
  void intersect(std::string name_new_tree, std::string name_tree1, std::string name_tree2, DataTree& set_trees);
  void union_data(std::string name_new_tree, std::string name_tree1, std::string name_tree2, DataTree& set_trees);
}
#endif
