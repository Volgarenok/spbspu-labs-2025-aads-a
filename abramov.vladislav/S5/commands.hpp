#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <functional>
#include <binary_tree/binary_tree.hpp>
#include "key_summer.hpp"

namespace abramov
{
  using CommandTree = BinarySearchTree< std::string, std::function< void() > >;
  using DataTree = BinarySearchTree< long long int, std::string >;

  template< class F >
  void ascending(DataTree &tree, F &f)
  {
    f = tree.traverse_lnr(f);
  }

  template< class F >
  void descending(DataTree &tree, F &f)
  {
    f = tree.traverse_rnl(f);
  }

  template< class F >
  void breadth(DataTree &tree, F &f)
  {
    f = tree.traverse_breadth(f);
  }

  template< class F >
  void getCommands(CommandTree &commands, DataTree &tree, F &f)
  {
    commands["ascending"] = std::bind(ascending< F >, std::ref(tree), std::ref(f));
    commands["descending"] = std::bind(descending< F >, std::ref(tree), std::ref(f));
    commands["breadth"] = std::bind(breadth< F >, std::ref(tree), std::ref(f));
  }
}
#endif
