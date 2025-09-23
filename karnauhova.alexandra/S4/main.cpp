#include <iostream>
#include <functional>
#include <cstring>
#include <fstream>
#include <map>
#include <stdexcept>
#include <limits>
#include "avl_tree.hpp"
#include "dataset_work.hpp"

namespace
{
  using TreeKey = karnauhova::AvlTree< size_t, std::string >;
  using DataTree = karnauhova::AvlTree< std::string, TreeKey >;
  void input_trees(std::ifstream& in, DataTree& map_trees)
  {
    std::string name_map;
    while (in >> name_map)
    {
      TreeKey tree_value;
      size_t key = 0;
      while (in >> key)
      {
        std::string value;
        in >> value;
        tree_value.insert(std::pair< size_t, std::string >{key, value});
      }
      map_trees.insert(std::pair< std::string, TreeKey >{name_map, tree_value});
      in.clear();
    }
  }
}

int main(int argc, char* argv[])
{
  using namespace karnauhova;
  using TreeKey = AvlTree< size_t, std::string >;
  using DataTree = AvlTree< std::string, TreeKey >;
  if (argc != 2)
  {
    std::cerr << "Incorrect file\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  DataTree set_trees;
  try
  {
    input_trees(file, set_trees);
  }
  catch (const std::exception& e)
  {
    std::cerr << "1" << e.what() << '\n';
    return 1;
  }
  while (!std::cin.eof())
  {
    std::string name_operat;
    std::cin >> name_operat;
    if (std::cin.eof())
    {
      break;
    }
    try
    {
      if (name_operat == "print")
      {
        std::string name_tree;
        std::cin >> name_tree;
        printDataset(std::cout, name_tree, set_trees);
      }
      else if (name_operat == "complement")
      {
        std::string name_new_tree;
        std::string name_tree1;
        std::string name_tree2;
        std::cin >> name_new_tree >> name_tree1 >> name_tree2;
        complement(name_new_tree, name_tree1, name_tree2, set_trees);
      }
      else if (name_operat == "intersect")
      {
        std::string name_new_tree;
        std::string name_tree1;
        std::string name_tree2;
        std::cin >> name_new_tree >> name_tree1 >> name_tree2;
        intersect(name_new_tree, name_tree1, name_tree2, set_trees);
      }
      else if (name_operat == "union")
      {
        std::string name_new_tree;
        std::string name_tree1;
        std::string name_tree2;
        std::cin >> name_new_tree >> name_tree1 >> name_tree2;
        unionData(name_new_tree, name_tree1, name_tree2, set_trees);
      }
      else
      {
        throw std::logic_error("Error in commands");
      }
    }
    catch (const std::exception& e)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
