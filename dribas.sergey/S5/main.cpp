#include <iostream>
#include <fstream>
#include <map>
#include <functional>
#include <avlTree.hpp>
#include "sumKey.hpp"

namespace
{
  dribas::SumKey traverse_lnr_wrap(dribas::AVLTree<long long, std::string>& tree, dribas::SumKey sum)
  {
    return tree.traverse_lnr(sum);
  }

  dribas::SumKey traverse_rnl_wrap(dribas::AVLTree<long long, std::string>& tree, dribas::SumKey sum)
  {
    return tree.traverse_rnl(sum);
  }

  dribas::SumKey traverse_breadth_wrap(dribas::AVLTree<long long, std::string>& tree, dribas::SumKey sum)
  {
    return tree.traverse_breadth(sum);
  }
}

int main(int argc, char** argv)
{
  if (argc != 3) {
    std::cerr << "Error!! Not enough arguments\n";
    return 1;
  }

  std::string mode(argv[1]);
  std::string filename(argv[2]);
  dribas::AVLTree<long long, std::string, std::less<long long>> tree;
  std::ifstream file(filename);

  if (!file) {
    std::cerr << "No file\n";
    return 1;
  }

  long long key = 0;
  std::string value;
  while (file >> key >> value) {
    tree.insert({key, value});
  }

  file.close();

  if (tree.empty()) {
    std::cout << "<EMPTY>\n";
    return 0;
  }

  std::map< std::string, std::function< dribas::SumKey(dribas::SumKey) > > commands;
  commands["ascending"] = std::bind(traverse_lnr_wrap, std::ref(tree), std::placeholders::_1);
  commands["descending"] = std::bind(traverse_rnl_wrap, std::ref(tree), std::placeholders::_1);
  commands["breadth"] = std::bind(traverse_breadth_wrap, std::ref(tree), std::placeholders::_1);

  try {
    dribas::SumKey key_sum;
    auto it = commands.find(mode);
    if (it != commands.end()) {
      key_sum = it->second(key_sum);
      std::cout << key_sum.key << " " << key_sum.value << "\n";
    } else {
      std::cerr << "Invalid mode\n";
      return 1;
    }
  } catch (const std::overflow_error& e) {
    std::cerr << e.what() << "\n";
    return 1;
  }

  return 0;
}
