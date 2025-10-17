#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <functional>
#include <string>
#include <tree.hpp>
#include "key_summer.hpp"

namespace guseynov
{
  using CommandTree = Tree< std::string, std::function< void() >, std::less< std::string > >;
  using DataTree = Tree< long long, std::string, std::less< long long > >;

  template< class F >
  void ascending(DataTree& tree, F& f)
  {
    f = tree.traverse_lnr(f);
  }

  template< class F >
  void descending(DataTree& tree, F& f)
  {
    f = tree.traverse_rnl(f);
  }

  template< class F >
  void breadth(DataTree& tree, F& f)
  {
    f = tree.traverse_breadth(f);
  }

  template< class F >
  void getCommands(CommandTree& commands, DataTree& tree, F& f)
  {
    commands.insert(std::make_pair("ascending", std::bind(ascending< F >, std::ref(tree), std::ref(f))));
    commands.insert(std::make_pair("descending", std::bind(descending< F >, std::ref(tree), std::ref(f))));
    commands.insert(std::make_pair("breadth", std::bind(breadth< F >, std::ref(tree), std::ref(f))));
  }
}

#endif
