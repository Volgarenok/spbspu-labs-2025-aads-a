#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <string>
#include <AVLtree.hpp>
#include "graph.hpp"

namespace tkach
{
  void printGraphs(std::ostream& out, const AvlTree< std::string, Graph >& tree_of_graphs);
  void printVertexes(std::ostream& out, std::istream& in, const AvlTree< std::string, Graph >& tree_of_graphs);
  void printOutbound(std::ostream& out, std::istream& in, const AvlTree< std::string, Graph >& tree_of_graphs);
  void printInbound(std::ostream& out, std::istream& in, const AvlTree< std::string, Graph >& tree_of_graphs);
  void bind(std::istream& in, AvlTree< std::string, Graph >& tree_of_graphs);
  void cut(std::istream& in, AvlTree< std::string, Graph >& tree_of_graphs);
  void create(std::istream& in, AvlTree< std::string, Graph >& tree_of_graphs);
  void merge(std::istream& in, AvlTree< std::string, Graph >& tree_of_graphs);
  void extract(std::istream& in, AvlTree< std::string, Graph >& tree_of_graphs);
}

#endif
